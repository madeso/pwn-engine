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
#include <pwn/assert.h>
#include <set>

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
				const real x = c->getNumber(0);
				const real y = c->getNumber(1);
				const real z = c->getNumber(2);
				const real w = c->getNumber(3);

				const math::quat q(x,y,z,w);
				return math::GetNormalized(q);
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
				math::vec3 vec() const
				{
					return math::vec3(0, length, 0);
				}
				pwn::string name;
				pwn::real length;
				math::quat orientation;
				Influence influence;
				std::vector<Bone> childs;
				std::vector<NamedObject> objects;
				math::mat44 xform;
				
				const Bone* getBone(const pwn::string& s) const
				{
					if( name == s ) return this;
					else
					{
						BOOST_FOREACH(const Bone& b, childs)
						{
							const Bone* fb = b.getBone(s);
							if( fb ) return fb;
						}
						return 0;
					}
				}
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

				Bone getBone(const pwn::string& name) const 
				{
					const Bone* bone = root.getBone(name);
					if( bone == 0 ) throw "missing bone";
					return *bone;
				}
			};

			Figure ExtractFigure(ChildPtr fig)
			{
				Figure r;
				r.name = fig->getString(0);
				r.root = ExtractBone(fig->getChild("bone"));
				return r;
			}

			struct FloatKey
			{
				int frame;
				pwn::real value;
			};

			FloatKey ExtractFloatKey(ChildPtr c)
			{
				FloatKey r;
				r.frame = c->getInt(0);
				r.value = c->getNumber(1);
				return r;
			}

			std::vector<FloatKey> ExtractFloatTrack(ChildPtr c)
			{
				std::vector<FloatKey> r;
				BOOST_FOREACH(ChildPtr fk, c->getChilds("floatkey"))
				{
					r.push_back(ExtractFloatKey(fk));
				}
				return r;
			}

			struct JointAngle
			{
				pwn::string bone;
				pwn::string axis;
				std::vector<FloatKey> keys;
			};

			JointAngle ExtractJointAngle(ChildPtr c)
			{
				JointAngle r;
				r.bone = c->getString(0);
				r.axis = c->getString(1);
				if( c->hasChild("track") ) r.keys = ExtractFloatTrack(c->getChild("track"));
				return r;
			}

			struct Sequence
			{
				pwn::string name;
				pwn::string figure;
				int frames;
				std::vector<JointAngle> joints;
			};

			Sequence ExtractSequence(ChildPtr c)
			{
				Sequence r;
				r.name = c->getString(0);
				if( c->hasChild("figure") ) r.figure = c->getChild("figure")->getString(0);
				r.frames = c->getChild("frames")->getInt(0);
				if( c->hasChild("jointangle") )
				{
					BOOST_FOREACH(ChildPtr j, c->getChilds("jointangle"))
					{
						r.joints.push_back(ExtractJointAngle(j));
					}
				}
				return r;
			}

			struct File
			{
				std::vector<Object> objects;
				std::map<pwn::string, pwn::string> textures;
				std::vector<Figure> figures;
				std::vector<Sequence> sequences;

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

				Figure getFigure(const pwn::string objectName) const
				{
					BOOST_FOREACH(const Figure& o, figures)
					{
						if( o.name == objectName )
						{
							return o;
						}
					}
					Throw(core::Str() << "Unable to find figure " << objectName);
					return Figure();
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
				if( file->hasChild("sequence") )
				{
					BOOST_FOREACH(ChildPtr c, file->getChilds("sequence"))
					{
						r.sequences.push_back(ExtractSequence(c));
					}
				}
				return r;
			}

			struct BoneIndexes
			{
				typedef std::map<const Bone*, mesh::BoneIndex> BoneIndexMap;
				BoneIndexMap map;

				void add(const Bone* b, mesh::BoneIndex i)
				{
					map.insert(BoneIndexMap::value_type(b,i));
				}

				mesh::BoneIndex get(const Bone* b) const
				{
					BoneIndexMap::const_iterator r = map.find(b);
					if( r == map.end() ) Throw(Str() << "Bone " << b->name << " has not been registred");
					return r->second;
				}
			};

			struct BoneAssignment
			{
				const Bone* parent;
				BoneIndexes* indexes;
				std::vector<const Bone*> bones;
			};

			struct BoneAssignmentBuilder
			{
				typedef std::pair<mesh::BoneIndex, real> Ba;
				struct BaCompare
				{
					bool operator()(const Ba& lhs, const Ba& rhs) const
					{
						return lhs.second > rhs.second;
					}
				};
				typedef std::multiset<Ba, BaCompare> BaSet;
				BaSet bas;
				void add(mesh::BoneIndex i, real v)
				{
					bas.insert(Ba(i,v));
				}
				math::vec4 getAssignment(mesh::BoneIndex bi) const
				{
					if( bas.empty() )
					{
						return math::vec4(bi+0.5f, -1, -1, -1);
					}
					real sum = 0;
					int index = 0;
					Ba b[4];
					BaSet bas = this->bas;
					const std::size_t basSize = bas.size();
					while(index<4 && !bas.empty())
					{
						BaSet::iterator i = bas.begin();
						sum += i->second;
						b[index] = *i;
						bas.erase(i);
						++index;
					}

					Assert( sum > 0 );

					for(;index<4;++index)
					{
						// when evaluating, this should result in a assignment of -1 (nothing)
						b[index] = Ba(0, -sum);
					}

					// if size = 1, C() returns the influence becomes val/val=100%, index gets trashed and influence goes to zero, this is why there is this special case
					if( basSize == 1 )
					{
						return math::vec4(b[0].first+0.5f, -1, -1, -1);
					}
					else
					{
						const math::vec4 r(C(b[0], sum), C(b[1], sum), C(b[2], sum), C(b[3], sum));
						return r;
					}
				}
				static real C(const Ba& b, real sum)
				{
					return b.first + b.second/sum;
				}
			};

			class Ray
			{
			public:
				Ray(const math::vec3& astart, const math::vec3& end)
					: start(astart)
					, dir(math::FromTo(astart, end))
				{
				}

				math::vec3 getPoint(real t) const
				{
					return start + dir*t;
				}

				real project(const math::vec3& p) const
				{
					const math::vec3 d = math::FromTo(start, p);
					return cml::dot(d, dir);
				}

				bool getDistance(const math::vec3& p, real* d, real* os) const
				{
					const real s = project(p);
					if( d )
					{
						const math::vec3 c = getPoint(s);
						*d = math::FromTo(c, p).length_squared();
					}
					if( os ) *os = s;

					return math::IsWithinInclusive(0, s, 1);
				}

			private:
				math::vec3 start;
				math::vec3 dir;
			};

			class Sphere
			{
			public:
				Sphere(const math::vec3& c, const real r)
					: center(c)
					, radiuss(r*r)
				{
				}

				const real getRadiusSquared() const
				{
					return radiuss;
				}

				real getDistanceToCenterSquared(const math::vec3& p) const
				{
					return math::FromTo(center, p).length_squared();
				}

			private:
				math::vec3 center;
				real radiuss;
			};

			class Capsule
			{
			public:
				Capsule(const math::vec3& astart, const math::vec3& aend, real r1, real r2)
					: start(astart, r1)
					, middle(astart, aend)
					, end(aend, r2)
				{
				}

				bool contains(const math::vec3& p, real* dist, real* range) const
				{
					real scale = 0;
					real d = 0;
					if( middle.getDistance(p, &d, &scale) )
					{
						*range = math::Lerp(start.getRadiusSquared(), scale, end.getRadiusSquared());
						*dist = d;
					}
					else if( scale >= 1 )
					{
						*range = end.getRadiusSquared();
						*dist = end.getDistanceToCenterSquared(p);
					}
					else
					{
						Assert(scale <= 0);
						*range = start.getRadiusSquared();
						*dist = start.getDistanceToCenterSquared(p);
					}

					return *dist < *range;
				}
			private:
				Sphere start;
				Ray middle;
				Sphere end;
			};

			real GetPossibleAssignment(const Bone* b, const math::vec3& xyz)
			{
				// todo
				const Influence& i = b->influence;
				math::mat44helper h(b->xform);
				Ray r(h.transform(math::vec3(0,0,0)), h.transform(math::vec3(0, 1, 0)));
				const math::vec3 start = r.getPoint(i.center0);
				const math::vec3 end = r.getPoint(i.center1);
				Capsule inner(start, end, i.inRadius0, i.inRadius1);
				Capsule outer(start, end, i.outRadius0, i.outRadius1);

				real innerdist = 0;
				real innerrange = 0;
				if( true == inner.contains(xyz, &innerdist, &innerrange) ) return 1;
				real outerdist = 0;
				real outerrange = 0;
				if( false == outer.contains(xyz, &outerdist, &outerrange) ) return 0;

				Assert(innerrange < outerrange);
				Assert(innerdist < outerrange);
				Assert(innerdist > innerrange);
				const real val = innerdist  - innerrange;
				Assert(val >= 0 );
				const real range = outerrange - innerrange;
				Assert(range > 0);
				const real inf = val / range;

				Assert(math::IsWithinInclusive(0, inf, 1));

				return inf;
			}

			math::vec4 GetBoneAssignment(BoneAssignment* a, const math::vec3& xyz)
			{
				const math::vec4 noBone(-1,-1,-1,-1);
				if( !a ) return noBone;
				BoneAssignmentBuilder bas;
				BOOST_FOREACH(const Bone* b, a->bones)
				{
					const real ass = GetPossibleAssignment(b, xyz);
					if( ass > 0 )
					{
						bas.add(a->indexes->get(b), ass);
					}
				}
				return bas.getAssignment( a->indexes->get(a->parent) );
			}

			class An8
			{
			public:
				File f;

				typedef std::map<pwn::string, int> NameIntMap;
				NameIntMap boundedMaterials;

				int getOrAddMaterial(const Object& o, pwn::mesh::Builder* builder, const pwn::string& name)
				{
					NameIntMap::iterator i = boundedMaterials.find(name);
					if( i != boundedMaterials.end() )
					{
						return i->second;
					}
					const int m = addMaterial(o, builder, name);
					boundedMaterials.insert( NameIntMap::value_type(name, m) );
					return m;
				}

				int addMaterial(const Object& o, pwn::mesh::Builder* builder, const pwn::string& name)
				{
					StringMaterialMap::const_iterator i = o.materials.find(name);
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

				void addToBuilder(const Object& o, pwn::mesh::Builder* b, math::mat44 mm, BoneAssignment* ba)
				{
					int pointBase = 0;
					int textureBase = 0;
					math::mat44helper mh(mm);
					BOOST_FOREACH(const Mesh& m, o.meshes)
					{
						BOOST_FOREACH(const math::vec3& v, m.points)
						{
							const math::vec3 xyz = mh.transform(v);
							b->addPosition(xyz, GetBoneAssignment(ba, xyz));
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
							b->addFace(getOrAddMaterial(o, b, m.materials[f.material]), toadd);
						}
						pointBase += m.points.size();
						textureBase += m.textcoords.size();
					}
				}

				void addToBuilder(const Figure& o, pwn::mesh::Builder* builder)
				{
					BoneIndexes bi;
					walkBone(0, o.root, builder, &bi, math::mat44Identity());
				}

				Figure& prepareFigure(Figure& f)
				{
					prepareBones(f.root, math::mat44Identity());
					return f;
				}

				void prepareBones(Bone& b, const math::mat44& root)
				{
					const math::mat44 rotatedr = math::mat44helper(root).rotate(b.orientation).mat;
					BOOST_FOREACH(Bone& c, b.childs)
					{
						prepareBones(c, math::mat44helper(rotatedr).translate(b.vec()).mat);
					}
					b.xform = rotatedr;
				}

				void walkBone(const Bone* parent, const Bone& b, pwn::mesh::Builder* builder, BoneIndexes* bi, const math::mat44& root)
				{
					mesh::Bone mb;
					if( parent ) mb.setParent(bi->get(parent));
					mb.setName(b.name);
					mb.rot = b.orientation;
					mb.pos = b.vec();
					const mesh::BoneIndex theid = builder->bones.size();
					builder->addBone(mb);
					bi->add(&b, theid);
					const math::mat44 rotatedr = math::mat44helper(root).rotate(b.orientation).mat;
					BOOST_FOREACH(const Bone& c, b.childs)
					{
						walkBone(&b, c, builder, bi, math::mat44helper(rotatedr).translate(b.vec()).mat);
					}

					BOOST_FOREACH(const NamedObject& n, b.objects)
					{
						BoneAssignment ba;
						ba.parent = &b;
						ba.indexes = bi;
						BOOST_FOREACH(const pwn::string& bname, n.weightedby)
						{
							ba.bones.push_back(b.getBone(bname));
						}
						addToBuilder(f.getObject(n.object), builder, rotatedr*n.base, &ba);
					}
				}
			};

			namespace
			{
				const pwn::string kObjectId = "!";
				const pwn::string kFigureId = "@";
			}

			void ExtractObjectNames(const File& f, std::set<pwn::string>* s)
			{
				BOOST_FOREACH(const Object& o, f.objects)
				{
					s->insert(kObjectId + o.name);
				}
			}

			void RemoveObjectNames(const Bone& b, std::set<pwn::string>* s)
			{
				BOOST_FOREACH(const NamedObject& n, b.objects)
				{
					std::set<pwn::string>::const_iterator r = s->find(kObjectId + n.object);
					if( r != s->end() )
					{
						s->erase(r);
					}
				}

				BOOST_FOREACH(const Bone& c, b.childs)
				{
					RemoveObjectNames(c, s);
				}
			}

			void ExtractFigureNames(const File& f, std::set<pwn::string>* s)
			{
				BOOST_FOREACH(const Figure& o, f.figures)
				{
					s->insert(kFigureId + o.name);
					RemoveObjectNames(o.root, s);
				}
			}

			struct An8KeyFrame
			{
				An8KeyFrame(int f, real av)
					: frame(f)
					, v(av)
				{
				}
				int frame;
				real v;
			};

			struct CompiledAn8Data
			{
				std::vector<An8KeyFrame> frames;

				bool has(int f) const
				{
					const std::size_t size = frames.size();
					for(std::size_t i=0; i<size; ++i)
					{
						if( frames[i].frame == f ) return true;
					}
					return false;
				}

				real get(int f) const
				{
					if( frames.empty() == true ) //throw "invalid data";
						return 0;
					const std::size_t size = frames.size();
					for(std::size_t i=0; i<size; ++i)
					{
						if( frames[i].frame == f ) return frames[i].v;
						if( frames[i].frame > f )
						{
							if( i == 0 ) return frames[i].v;
							else return math::Remap(frames[i-1].frame, frames[i].frame, f, frames[i-1].v, frames[i].v);
						}
					}
					return frames[size-1].v;
				}
			};

			std::size_t GetBoneIndex(const mesh::Builder& b, const pwn::string name)
			{
				const std::size_t count = b.bones.size();
				for(std::size_t i=0;i<count; ++i)
				{
					if( b.bones[i].getName() == name )
						return i;
				}
				throw "unknown bone name, use shorter bone names= consider rewriting lookup?";
			}

			mesh::Animation ExtractSequence(const File& file, const mesh::Builder& b, const Sequence& s)
			{
				std::vector<CompiledAn8Data> cdata[3]; // x, y & z

				// resize, so that we can index this as we please later on
				for(int i=0;i<3; ++i)
				{
					cdata[i].resize(b.bones.size());
				}

				const Figure& figure = file.getFigure(s.figure);

				BOOST_FOREACH(const JointAngle& ja, s.joints)
				{
					int diff = -1;
					if("X" == ja.axis)
					{
						diff = 0;
					}
					else if("Y" == ja.axis)
					{
						diff = 1;
					}
					else if("Z" == ja.axis)
					{
						diff = 2;
					}
					else
					{
						throw "unknown axis";
					}

					const std::size_t index = GetBoneIndex(b, ja.bone);
					CompiledAn8Data& data = cdata[diff][index];
					const Bone b = figure.getBone(ja.bone);

					BOOST_FOREACH(const FloatKey& fk, ja.keys)
					{
						data.frames.push_back( An8KeyFrame(fk.frame, fk.value) );
					}

					//math::vec3 originalOrientation = b.orientation;
					//data.addFirst(originalOrientation[diff]);
				}

				const std::size_t numbones = b.bones.size();
				std::vector<mesh::AnimationPerBone> bones(numbones);
				for(int bone=0; bone<numbones; ++bone)
				{
					std::vector<mesh::FramePosition> afp;
					std::vector<mesh::FrameRotation> afr;

					for(int frame=0; frame<s.frames; ++frame)
					{
						if( cdata[0][bone].has(frame) || cdata[1][bone].has(frame) || cdata[2][bone].has(frame) )
						{
							const real x = cdata[0][bone].get(frame);
							const real y = cdata[1][bone].get(frame);
							const real z = cdata[2][bone].get(frame);
							afp.push_back(mesh::FramePosition(frame, b.bones[bone].pos));
							afr.push_back(mesh::FrameRotation(frame, math::cquat(x,y,z)));
						}
					}

					if( afp.empty() )
					{
						afp.push_back(mesh::FramePosition(0, b.bones[bone].pos));
					}
					if( afr.empty() )
					{
						afr.push_back(mesh::FrameRotation(0, b.bones[bone].rot));
					}

					bones[bone]= mesh::AnimationPerBone(afp, afr);
				}

				return mesh::Animation(bones);
			}

			void LoadAnimations(AnimationList* animations, const File& f, const mesh::Builder& builder)
			{
				BOOST_FOREACH(const Sequence& s, f.sequences)
				{
					mesh::Animation animation = ExtractSequence(f, builder, s);
					animations->push_back(AnimationEntry(animation, s.name));
				}
			}

			void read(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& path)
			{
				File f = ExtractFile(Load(path));

				std::set<pwn::string> subs;
				BOOST_FOREACH(pwn::string s, subobjects)
				{
					subs.insert(s);
				}
				
				if( subs.empty() )
				{
					ExtractObjectNames(f, &subs);
					ExtractFigureNames(f, &subs);
				}

				BOOST_FOREACH(pwn::string& name, subs)
				{
					const pwn::string id = name.substr(0, 1);
					if( id == kObjectId )
					{
						const pwn::string objectName = name.substr(1);
						An8 a;
						a.f = f;
						mesh::Builder builder;
						a.addToBuilder(a.f.getObject(objectName), &builder, math::mat44Identity(), 0);
						builders->push_back(Entry(builder, objectName));
					}
					else if( id == kFigureId )
					{
						const pwn::string figName = name.substr(1);
						An8 a;
						a.f = f;
						mesh::Builder builder;
						a.addToBuilder(a.prepareFigure(a.f.getFigure(figName)), &builder);
						Entry e(builder, figName);
						LoadAnimations(&e.animations, f, builder);
						builders->push_back(e);
					}
					else
					{
						Throw(core::Str() << "Unknown type " << id << " from " << name);
					}
				}
			}
		}
	}
}
