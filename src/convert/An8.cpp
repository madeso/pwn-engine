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
#include <pwn/math/operations.h>
#include <pwn/mesh/builder.h>

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
					, ignoreChar(0)
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
						if( ignoreChar>0 || ignoreChar==-1 )
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
								if( IsNumber(c) )
								{
									if( ignoreChar == -1 )
									{
										ignoreChar = 3;
									}
								}
								else
								{
									Throw(Str() << "(" << line << ") unknown escape " << c);
								}
								break;
							}
							if( ignoreChar == -1 ) ignoreChar = 0;
							else --ignoreChar;
						}
						else
						{
							if( c == '\\' )
							{
								ignoreChar = -1;
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
				int ignoreChar; // -1 means ignore one character and thats it, bigger than 0 is a countdown to stop ignoring, 0 means no ignore
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
				if( m->hasChild("texcoords") )
				{
					r.textcoords = ExtractTextcoords(m->getChild("texcoords"));
				}
				r.faces = ExtractFaces(m->getChild("faces"));
				r.materials = ExtractMaterials(m->getChild("materiallist"));
				return r;
			}

			struct Color
			{
				Color()
					: rgb(0,0,0)
				{
				}
				Color(const math::Rgba& r)
					: rgb(r.r, r.g, r.b)
				{
				}
				pwn::math::Rgb rgb;
				pwn::string texture;
			};

			Color ExtractColor(ChildPtr m)
			{
				Color c;
				ChildPtr rgb = m->getChild("rgb");
				c.rgb = pwn::math::Rgb(rgb->getNumber(0)/255, rgb->getNumber(1)/255, rgb->getNumber(2)/255);
				if( m->hasChild("texturename") )
				{
					c.texture = m->getChild("texturename")->getString(0);
				}
				return c;
			}

			struct Material
			{
				pwn::string name;
				Color ambiant;
				Color diffuse;
				Color specular;
				Color emissive;
				real alpha;
			};

			Material ExtractMaterial(ChildPtr ma)
			{
				Material m;
				m.name = ma->getString(0);

				const pwn::mesh::Material d;

				ChildPtr s = ma->getChild("surface");
				m.ambiant = ExtractColor(s->getChild("ambiant"));
				m.diffuse = ExtractColor(s->getChild("diffuse"));
				m.specular = s->hasChild("specular") ? ExtractColor(s->getChild("specular")) : d.specular;
				m.alpha = s->hasChild("alpha") ? s->getChild("alpha")->getNumber(0)/255.0f : 1.0f;
				m.emissive = s->hasChild("emissive") ? ExtractColor(s->getChild("emissive")) : d.emission;

				return m;
			}

			typedef std::map<pwn::string, Material> StringMaterialMap;

			struct Object
			{
				std::vector<Mesh> meshes;
				StringMaterialMap materials;
				pwn::string name;
			};

			void ExtractMeshOnObject(Object& r, ChildPtr o, const pwn::string& type)
			{
				if( o->hasChild(type) )
				{
					BOOST_FOREACH(ChildPtr m, o->getChilds(type))
					{
						r.meshes.push_back(ExtractMesh(m));
					}
				}
			}

			Object ExtractObject(ChildPtr o)
			{
				Object r;
				r.name = o->getString(0);
				ExtractMeshOnObject(r, o, "mesh");
				ExtractMeshOnObject(r, o, "subdivision");
				BOOST_FOREACH(ChildPtr m, o->getChilds("material"))
				{
					const Material mat = ExtractMaterial(m);
					r.materials.insert( StringMaterialMap::value_type(mat.name, mat) );
				}
				return r;
			}

			struct Influence
			{
				Influence()
					: center0(0)
					, inRadius0(0)
					, outRadius0(0)
					, center1(0)
					, inRadius1(0)
					, outRadius1(0)

				{
				}
				pwn::real center0; //The location along the bone of the center of the lower end of the influence volume.
				pwn::real inRadius0; //The radius of the inner layer at center0.
				pwn::real outRadius0; //The radius of the outer layer at center0.
				pwn::real center1; //The location along the bone of the center of the uppper end of the influence volume.
				pwn::real inRadius1; //The radius of the inner layer at center1.
				pwn::real outRadius1; //The radius of the outer layer at center1.
			};

			Influence ExtractInfluence(ChildPtr c)
			{
				Influence r;
				r.center0 = c->getNumber(0);
				r.inRadius0 = c->getNumber(1);
				r.outRadius0 = c->getNumber(2);
				r.center1 = c->getNumber(3);
				r.inRadius1 = c->getNumber(4);
				r.outRadius1 = c->getNumber(5);
				return r;
			}

			math::quat ExtractQuaternion(ChildPtr c)
			{
				return math::quat(c->getNumber(0), c->getNumber(1), c->getNumber(2), c->getNumber(3));
			}

			math::mat44 ExtractBase(ChildPtr owner, const pwn::string& name)
			{
				if( owner->hasChild(name) == false) return math::mat44Identity();
				ChildPtr c = owner->getChild(name);
				ChildPtr o = c->getChild("origin");
				const math::vec3 origin(o->getNumber(0), o->getNumber(1), o->getNumber(2));
				const math::quat orientation = ExtractQuaternion( c->getChild("orientation") );
				return math::mat44helper(math::mat44Identity()).rotate(orientation).translate(-origin).mat;
			}

			struct NamedObject
			{
				pwn::string name;
				pwn::string object;
				math::mat44 base;
				std::vector<pwn::string> weightedby;
			};

			NamedObject ExtractNamedObject(ChildPtr n)
			{
				NamedObject r;
				r.name = n->getString(0);
				r.object = n->getChild("name")->getString(0);
				r.base = ExtractBase(n, "base");
				if( n->hasChild("weightedby") )
				{
					BOOST_FOREACH(ChildPtr c, n->getChilds("weightedby"))
					{
						r.weightedby.push_back(c->getString(0));
					}
				}
				return r;
			}

			struct Bone
			{
				Bone()
					: length(0)
					, orientation( math::qIdentity() )
				{
				}
				pwn::string name;
				pwn::real length;
				math::quat orientation;
				Influence influence;
				std::vector<Bone> childs;
				std::vector<NamedObject> objects;
			};

			Bone ExtractBone(ChildPtr bone)
			{
				Bone r;
				r.name = bone->getString(0);
				r.length = bone->getChild("length")->getNumber(0);
				if( bone->hasChild("influence") ) r.influence = ExtractInfluence(bone->getChild("influence"));
				if( bone->hasChild("orientation") ) r.orientation = ExtractQuaternion(bone->getChild("orientation"));
				if( bone->hasChild("bone") )
				{
					BOOST_FOREACH(ChildPtr s, bone->getChilds("bone") )
					{
						r.childs.push_back(ExtractBone(s));
					}
				}
				if( bone->hasChild("namedobject") )
				{
					BOOST_FOREACH(ChildPtr n, bone->getChilds("namedobject"))
					{
						r.objects.push_back(ExtractNamedObject(n));
					}
				}
				return r;
			}

			struct Figure
			{
				pwn::string name;
				Bone root;
			};

			Figure ExtractFigure(ChildPtr fig)
			{
				Figure r;
				r.name = fig->getString(0);
				r.root = ExtractBone(fig->getChild("bone"));
				return r;
			}

			struct File
			{
				std::vector<Object> objects;
				std::map<pwn::string, pwn::string> textures;
				std::vector<Figure> figures;

				Object getObject(const pwn::string objectName) const
				{
					BOOST_FOREACH(const Object& o, objects)
					{
						if( o.name == objectName )
						{
							return o;
						}
					}
					Throw(core::Str() << "Unable to find object " << objectName);
					return Object();
				}
			};

			std::map<pwn::string, pwn::string> ExtractTextures(ChildPtr file)
			{
				std::map<pwn::string, pwn::string> r;
				if( file->hasChild("texture") )
				{
					BOOST_FOREACH(ChildPtr c, file->getChilds("texture"))
					{
						r[c->getString(0)] = c->getChild("file")->getString(0);
					}
				}
				return r;
			}

			File ExtractFile(ChildPtr file)
			{
				File r;
				r.textures = ExtractTextures(file);
				BOOST_FOREACH(ChildPtr c, file->getChilds("object"))
				{
					r.objects.push_back(ExtractObject(c));
				}
				if( file->hasChild("figure") )
				{
					BOOST_FOREACH(ChildPtr c, file->getChilds("figure"))
					{
						r.figures.push_back(ExtractFigure(c));
					}
				}
				return r;
			}

			class An8
			{
			public:
				File f;
				Object o;

				typedef std::map<pwn::string, int> NameIntMap;
				NameIntMap boundedMaterials;

				int getOrAddMaterial(pwn::mesh::Builder* builder, const pwn::string& name)
				{
					NameIntMap::iterator i = boundedMaterials.find(name);
					if( i != boundedMaterials.end() )
					{
						return i->second;
					}
					const int m = addMaterial(builder, name);
					boundedMaterials.insert( NameIntMap::value_type(name, m) );
					return m;
				}

				int addMaterial(pwn::mesh::Builder* builder, const pwn::string& name)
				{
					StringMaterialMap::iterator i = o.materials.find(name);
					if( i == o.materials.end() ) Throw(Str() << "mesh uses unknown material " << name);
					pwn::mesh::Material m;
					const real alpha = i->second.alpha;
					m.ambient = pwn::math::Rgba(i->second.ambiant.rgb, alpha);
					m.diffuse = pwn::math::Rgba(i->second.diffuse.rgb, alpha);
					m.specular = pwn::math::Rgba(i->second.specular.rgb, alpha);
					m.emission = pwn::math::Rgba(i->second.emissive.rgb, alpha);
					pwn::string texname = i->second.diffuse.texture;
					m.setTexture_Diffuse(f.textures[texname]);
					return builder->addMaterial(name, m);
				}

				void addToBuilder(pwn::mesh::Builder* b)
				{
					int pointBase = 0;
					int textureBase = 0;
					BOOST_FOREACH(const Mesh& m, o.meshes)
					{
						const math::vec4 noBone(-1,-1,-1,-1);
						BOOST_FOREACH(const math::vec3& xyz, m.points)
						{
							b->addPosition(xyz, noBone);
						}
						BOOST_FOREACH(const math::vec2& uv, m.textcoords)
						{
							b->addTextCoord(uv);
						}
						BOOST_FOREACH(const Face& f, m.faces)
						{
							std::vector<pwn::mesh::BTriangle::Vertex> verts;
							BOOST_FOREACH(const Point& p, f.points)
							{
								verts.push_back(pwn::mesh::BTriangle::Vertex(pointBase+p.point, 0, textureBase + p.texture));
							}
							std::vector<pwn::mesh::BTriangle::Vertex> toadd(verts.rbegin(), verts.rend());
							b->addFace(getOrAddMaterial(b, m.materials[f.material]), toadd);
						}
						pointBase += m.points.size();
						textureBase += m.textcoords.size();
					}
				}
			};

			std::vector<pwn::string> ExtractObjectNames(const File& f)
			{
				std::vector<pwn::string> r;
				BOOST_FOREACH(const Object& o, f.objects)
				{
					r.push_back(o.name);
				}
				return r;
			}

			void read(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& path)
			{
				File f = ExtractFile(Load(path));

				std::vector<pwn::string> subs = subobjects;
				if( subs.empty() )
				{
					subs = ExtractObjectNames(f);
				}

				BOOST_FOREACH(pwn::string& objectName, subs)
				{
					An8 a;
					a.f = f;
					a.o = a.f.getObject(objectName);
					mesh::Builder builder;
					a.addToBuilder(&builder);
					builders->push_back(Entry(builder, objectName));
				}
			}
		}
	}
}