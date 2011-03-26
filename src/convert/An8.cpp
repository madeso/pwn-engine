#include "An8.hpp"

#include <vector>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <pwn/core/stdutil.h>
#include <pwn/number.h>

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
					Number, String, Ident, Begin, End
				};

				Term(Type t)
					: type(t)
				{
				}

				Term(Type t, const pwn::string& val)
					: type(t)
					, value(val)
				{
				}

				Type type;
				pwn::string value;
			};

			class Lexer
			{
			public:
				enum State
				{
					sNone, sIdent, sString, sNumber, sNumberWithoutDot
				};

				void on(char c)
				{
					switch(state)
					{
					case sNone:
						if( IsWhitespace(c) ) return;
						if( IsLetter(c) )
						{
							memory += c;
							state = sIdent;
						}
						else if( IsNumber(c) )
						{
							memory += c;
							state = sNumber;
						}
						else if( c == '{' )
						{
							terms.push_back(Term(Term::Begin));
						}
						else if( c == '}' )
						{
							terms.push_back(Term(Term::End));
						}
						else if ( c == '\"' )
						{
							state = sString;
						}
						else 
						{
							throw "not a known sign";
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
								throw "unknown escape";
								break;
							}
						}
						else
						{
							if( c == '\\' )
							{
								ignoreChar = true;
							}
							else if ( c == '\"' )
							{
								terms.push_back(Term(Term::String, memory));
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
							terms.push_back(Term(Term::Number, memory));
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
							terms.push_back(Term(Term::Number, memory));
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
							terms.push_back(Term(Term::Ident, memory));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					default:
						throw "unknown lexer state";
					}
				}

				void complete()
				{
					if( state == sNone ) return;
					if( state == sString ) throw "invalid string syntax";
					if( state == sNumber || state == sNumberWithoutDot )
					{
						terms.push_back(Term(Term::Number, memory));
						memory.clear();
						state = sNone;
					}
					if( state == sIdent )
					{
						terms.push_back(Term(Term::Ident, memory));
						memory.clear();
						state = sNone;
					}
					throw "unknown ending state";
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

				bool peek(Term* term, unsigned int steps=1) const
				{
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
					if( r == childs.end() ) throw "child not found";
					if( r->second.size() != 1 ) throw "no unique child";
					return r->second[0];
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
				typedef std::vector<std::string> ValueList;
				typedef std::vector<ChildPtr> ChildList;
				typedef std::map<pwn::string, ChildList> ChildMap;

				ChildMap childs;
				ValueList values;
				pwn::string name;
			};

			void ParseChildren(ChildPtr child, TermReader* tr)
			{
				Term t(Term::Begin);
				bool searchValues = true;
				while(tr->peek(&t) && t.type != Term::End)
				{
					switch(t.type)
					{
					case Term::Ident:
						{
							ChildPtr c(new Child(t.value));
							if( tr->read().type != Term::Begin ) throw "Syntax error, expecting a BEGIN";
							ParseChildren(c, tr);
							if( tr->read().type != Term::End ) throw "Syntax error, expecting a END";
						}
						break;
					case Term::Number:
						if( searchValues == false ) throw "Syntax error, not expecting a Number";
						child->addValue(t.value);
						break;
					case Term::String:
						if( searchValues == false ) throw "Syntax error, not expecting a String";
						child->addValue(t.value);
						break;
					case Term::Begin:
						throw "Syntax error, not expecting a BEGIN";
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
				if( f.good() == false ) throw "failed to open file";
				char c;
				Lexer lex;
				while( f >> c)
				{
					lex.on(c);
				}
				lex.complete();

				TermReader tr(lex.terms);
				return Parse(&tr);
			}

			typedef core::Vec<pwn::string> SVec;

			void read(pwn::mesh::Builder* builder, const pwn::string& path)
			{
				ChildPtr file = Load(path);
				const pwn::string version = file->getChild(SVec() << "header" << "version")->getString();
				const pwn::string build = file->getChild(SVec() << "header" << "build")->getString();
			}
		}
	}
}