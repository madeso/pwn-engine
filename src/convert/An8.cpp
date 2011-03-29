#include "An8.hpp"

#include <vector>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <pwn/core/stdutil.h>
#include <pwn/number.h>
#include <pwn/core/str.h>
#include <pwn/math/types.h>

namespace pwn
{
	namespace convert
	{
		namespace an8
		{
			struct Term
			{
				enum Type
				{
					Unknown, Number, String, Ident, Begin, End
				};

				Term()
					: type(Unknown)
					, line(0)
				{
				}

				Term(Type t, unsigned int l)
					: type(t)
					, line(l)
				{
				}

				Term(Type t, const pwn::string& val, unsigned int l)
					: type(t)
					, value(val)
					, line(l)
				{
				}

				Type type;
				pwn::string value;
				unsigned int line;
			};

			namespace
			{
				using namespace pwn::core;
				void Throw(const Str& str)
				{
					throw static_cast<pwn::string>(str);
				}
			}

			Str& operator<<(Str& str, const Term& t)
			{
				str << "(";

				switch(t.type)
				{
				case Term::Number:
					str << "NUMBER " << t.value;
					break;
				case Term::String:
					str << "STRING " << t.value;
					break;
				case Term::Ident:
					str << " " << t.value;
					break;
				case Term::Begin:
					str << "BEGIN";
					break;
				case Term::End:
					str << "END";
					break;
				}

				if( t.line != 0 ) str << " @ " << t.line;

				str << ")";
				return str;
			}

			class Lexer
			{
			public:
				unsigned int line;
				bool inComment;

				Lexer()
					: state(sNone)
					, line(1)
					, ignoreChar(false)
					, inComment(false)
				{
				}

				enum State
				{
					sNone, sIdent, sString, sNumber, sNumberWithoutDot
				};

				void on(char c)
				{
					if( c == '\n' ) ++line;
					if( inComment )
					{
						// todo: improve /**/ comment parsing
						if( c == '/' ) inComment = false;
						return;
					}
					switch(state)
					{
					case sNone:
						if( IsWhitespace(c) ) return;
						if( IsLetter(c) )
						{
							memory += c;
							state = sIdent;
						}
						else if( IsNumber(c) || c=='-')
						{
							memory += c;
							state = sNumber;
						}
						else if( c == '{' )
						{
							terms.push_back(Term(Term::Begin, line));
						}
						else if( c == '}' )
						{
							terms.push_back(Term(Term::End, line));
						}
						else if ( c == '\"' )
						{
							state = sString;
						}
						else if ( c == '/' )
						{
							inComment = true;
						}
						else if ( c == '(' )
						{
							// ignore parantesis
						}
						else if ( c == ')' )
						{
							// ignore parantesis
						}
						else 
						{
							Throw(Str() << "(" << line << ") not a known sign " << c);
						}
						break;
					case sString:
						if( ignoreChar )
						{
							switch(c)
							{
							case 't':
								memory += '\t';
								break;
							case 'n':
								memory += '\n';
								break;
							case '\"':
								memory += '\"';
								break;
							case '\\':
								memory += '\\';
								break;
							default:
								Throw(Str() << "(" << line << ") unknown escape " << c);
								break;
							}
							ignoreChar = false;
						}
						else
						{
							if( c == '\\' )
							{
								ignoreChar = true;
							}
							else if ( c == '\"' )
							{
								terms.push_back(Term(Term::String, memory, line));
								memory.clear();
								state = sNone;
							}
							else
							{
								memory += c;
							}
						}
						break;
					case sNumber:
						if( IsNumber(c) )
						{
							memory += c;
						}
						else if( c == '.' )
						{
							memory += '.';
							state = sNumberWithoutDot;
						}
						else
						{
							terms.push_back(Term(Term::Number, memory, line));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					case sNumberWithoutDot:
						if( IsNumber(c) )
						{
							memory += c;
						}
						else
						{
							terms.push_back(Term(Term::Number, memory, line));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					case sIdent:
						if( IsLetter(c) || IsNumber(c) )
						{
							memory += c;
						}
						else
						{
							terms.push_back(Term(Term::Ident, memory, line));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					default:
						Throw(Str() << "(" << line << ") Internal error: unknown lexer state " << state);
					}
				}

				void complete()
				{
					if( state == sNone ) return;
					if( state == sString ) throw "(eof) invalid string syntax";
					if( state == sNumber || state == sNumberWithoutDot )
					{
						terms.push_back(Term(Term::Number, memory, line));
						memory.clear();
						state = sNone;
					}
					if( state == sIdent )
					{
						terms.push_back(Term(Term::Ident, memory, line));
						memory.clear();
						state = sNone;
					}
					Throw(Str() << "(eof) unknown ending state" << state);
				}

				std::vector<Term> terms;
			private:
				bool ignoreChar;
				State state;
				pwn::string memory;

				bool IsWhitespace(char c)
				{
					const static pwn::string s(" \n\t");
					return s.find_first_of(c) != -1;
				}
				bool IsLetter(char c)
				{
					const static pwn::string s("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
					return s.find_first_of(c) != -1;
				}
				bool IsNumber(char c)
				{
					const static pwn::string s("0123456789");
					return s.find_first_of(c) != -1;
				}
			};

			class TermReader
			{
			public:
				TermReader(const std::vector<Term>& t)
					: terms(t)
					, index(0)
					, count(t.size())
				{
				}

				Term read()
				{
					if( eof() ) throw "end of file";
					const Term t = terms[index];
					++index;
					return t;
				}

				bool peek(Term* term, unsigned int asteps=1) const
				{
					const unsigned int steps = asteps -1;
					if( index+steps >= count ) return false;
					else
					{
						*term = terms[index+steps];
						return true;
					}
				}

				bool eof() const
				{
					return index >= count;
				}

			private:
				std::vector<Term> terms;
				std::size_t index;
				std::size_t count;
			};

			class Child;
			typedef boost::shared_ptr<Child> ChildPtr;

			class Child
			{
			public:
				typedef std::vector<std::string> ValueList;
				typedef std::vector<ChildPtr> ChildList;
				typedef std::map<pwn::string, ChildList> ChildMap;

				explicit Child(const pwn::string& n="")
					: name(n)
				{
				}

				pwn::string getString(size_t i=0) const
				{
					return values[i];
				}
				pwn::real getNumber(size_t i=0) const
				{
					return boost::lexical_cast<pwn::real>(getString(i));
				}
				int getInt(size_t i=0) const
				{
					return boost::lexical_cast<int>(getString(i));
				}
				size_t getValueCount() const
				{
					return values.size();
				}

				const pwn::string& getName() const
				{
					return name;
				}

				void addValue(const std::string& val)
				{
					values.push_back(val);
				}

				void addChild(ChildPtr child)
				{
					const pwn::string name = child->getName();
					ChildMap::iterator r = childs.find(name);
					
					ChildList& list = (r != childs.end()) ? r->second // if we found one, get a reference to the list
						: childs.insert(ChildMap::value_type(name, ChildList())).first->second; // if not create one and get the reference
					list.push_back(child);
				}
				
				ChildPtr getChild(const pwn::string& name)
				{
					ChildMap::iterator r = childs.find(name);
					if( r == childs.end() ) Throw(Str() << "child not found: " << name);
					if( r->second.size() != 1 ) Throw(Str() << "no unique child when searching for " << name);
					return r->second[0];
				}

				const ChildList& getChilds(const pwn::string& name) const
				{
					ChildMap::const_iterator r = childs.find(name);
					if( r == childs.end() ) Throw(Str() << "childs not found: " << name);
					return r->second;
				}

				ChildList& getChilds(const pwn::string& name)
				{
					ChildMap::iterator r = childs.find(name);
					if( r == childs.end() ) Throw(Str() << "childs not found: " << name);
					return r->second;
				}

				bool hasChild(const pwn::string& name) const
				{
					ChildMap::const_iterator r = childs.find(name);
					return r != childs.end();
				}

				ChildPtr getChild(const std::vector<pwn::string>& names)
				{
					ChildPtr ch = getChild(names[0]);
					for(std::size_t i=1; i<names.size(); ++i)
					{
						ch = ch->getChild(names[i]);
					}
					return ch;
				}
			private:
				ChildMap childs;
				ValueList values;
				pwn::string name;
			};

			void ParseChildren(ChildPtr child, TermReader* tr)
			{
				Term t;
				bool searchValues = true;
				while(tr->peek(&t) && t.type != Term::End)
				{
					t = tr->read();
					switch(t.type)
					{
					case Term::Ident:
						{
							Term next;
							if( tr->peek(&next) && next.type != Term::Begin)
							{
								child->addValue(t.value);
							}
							else
							{
								ChildPtr c(new Child(t.value));
								const Term start = tr->read();
								if( start.type != Term::Begin ) Throw(Str() << "Syntax error, expecting a BEGIN got " << start);
								ParseChildren(c, tr);
								const Term end = tr->read();
								if( end.type != Term::End ) Throw(Str() << "Syntax error, expecting a END got " << end);
								child->addChild(c);
							}
						}
						break;
					case Term::Number:
						if( searchValues == false ) Throw(Str() << "Syntax error, not expecting a Number, got " << tr);
						child->addValue(t.value);
						break;
					case Term::String:
						if( searchValues == false ) Throw(Str() << "Syntax error, not expecting a String, got " << tr);
						child->addValue(t.value);
						break;
					case Term::Begin:
						Throw(Str() << "Syntax error, not expecting a BEGIN, got " << tr);
						break;
					}
				}
			}

			ChildPtr Parse(TermReader* tr)
			{
				ChildPtr ptr(new Child());
				ParseChildren(ptr, tr);
				return ptr;
			}

			ChildPtr Load(const pwn::string& file)
			{
				std::ifstream f(file.c_str());
				if( f.good() == false ) Throw(Str() << "failed to open file: " << file);
				char c;
				Lexer lex;
				while( f >> std::noskipws >> c)
				{
					lex.on(c);
				}
				lex.complete();

				TermReader tr(lex.terms);
				return Parse(&tr);
			}

			typedef core::Vec<pwn::string> SVec;

			std::vector<pwn::math::vec3> ExtractPoints(ChildPtr points)
			{
				std::vector<pwn::math::vec3> r;
				const size_t count = points->getValueCount();
				for(size_t i=0; i<count; i+=3)
				{
					r.push_back( pwn::math::vec3(points->getNumber(i), points->getNumber(i+1), points->getNumber(i+2)) );
				}
				return r;
			}
			std::vector<pwn::math::vec2> ExtractTextcoords(ChildPtr points)
			{
				std::vector<pwn::math::vec2> r;
				const size_t count = points->getValueCount();
				for(size_t i=0; i<count; i+=2)
				{
					r.push_back( pwn::math::vec2(points->getNumber(i), points->getNumber(i+1)) );
				}
				return r;
			}
			enum 
			{
				SF_SHOW_BACK=1, SF_HAS_NORMALS=2, SF_HAS_TEXTURE=4
			};

			struct Point
			{
				int point;
				int normal;
				int texture;
			};

			struct Face
			{
				int material;
				int normalIndex;
				std::vector<Point> points;
			};

			std::vector<Face> ExtractFaces(ChildPtr faces)
			{
				std::vector<Face> r;
				const size_t count = faces->getValueCount();
				for(size_t i=0; i<count;)
				{
					const int points = faces->getInt(i);
					const int flags = faces->getInt(i+1);
					const int materialId = faces->getInt(i+2);
					const int normalIndex = faces->getInt(i+3);
					i+= 4;
					Face f;
					f.material = materialId;
					f.normalIndex = normalIndex;
					for(int p=0;p<points;++p)
					{
						const int point = faces->getInt(i);++i;
						int normal = -1;
						int texture = -1;
						if(flags & SF_HAS_NORMALS )
						{
							normal = faces->getInt(i);
							++i;
						}
						if(flags & SF_HAS_TEXTURE )
						{
							texture = faces->getInt(i);
							++i;
						}
						Point pn;
						pn.point = point;
						pn.normal = normal;
						pn.texture = texture;
						f.points.push_back(pn);
					}
					r.push_back(f);
				}
				return r;
			}

			std::vector<pwn::string> ExtractMaterials(ChildPtr materials)
			{
				std::vector<pwn::string> r;
				
				BOOST_FOREACH(ChildPtr m, materials->getChilds("materialname"))
				{
					r.push_back(m->getString());
				}
				return r;
			}

			struct Mesh
			{
				std::vector<pwn::math::vec3> points;
				std::vector<pwn::math::vec2> textcoords;
				std::vector<Face> faces;
				std::vector<pwn::string> materials;
			};

			Mesh ExtractMesh(ChildPtr m)
			{
				Mesh r;
				r.points = ExtractPoints(m->getChild("points"));
				r.textcoords = ExtractTextcoords(m->getChild("texcoords"));
				r.faces = ExtractFaces(m->getChild("faces"));
				r.materials = ExtractMaterials(m->getChild("materiallist"));
				return r;
			}

			void read(pwn::mesh::Builder* builder, const pwn::string& path)
			{
				ChildPtr file = Load(path);
				const pwn::string objectName = "sphere";

				BOOST_FOREACH(ChildPtr c, file->getChilds("object"))
				{
					if( c->getString(0) != objectName ) continue;
					std::vector<Mesh> meshes;

					BOOST_FOREACH(ChildPtr m, c->getChilds("mesh"))
					{
						meshes.push_back(ExtractMesh(m));
					}
				}
			}
		}
	}
}