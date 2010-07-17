#include <fstream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "WavefrontObj.hpp"
#include "Converter.hpp"

#include <pwn/core/StringUtils.h>
#include <pwn/math/operations.h>

#include <pwn/mesh/material.h>
#include <pwn/mesh/builder.h>

#include <pwn/mesh/Mesh.h>

#include <iostream>

namespace pwn
{
	namespace convert
	{
		namespace obj
		{
			namespace // local
			{
				std::size_t csizet(const pwn::string& str)
				{
					return boost::lexical_cast<std::size_t>(str);
				}
				pwn::real creal(const pwn::string& str)
				{
					return boost::lexical_cast<pwn::real>(str);
				}

				const mesh::Triangle::Vertex cFaceIndex(const pwn::string& astr)
				{
					const pwn::string str = pwn::core::Trim(astr);
					const pwn::string::size_type a = str.find_first_of('/');
					if( a == pwn::string::npos ) throw "bad face format 1";
					const pwn::string::size_type b = str.find_first_of('/', a+1);
					if( b == pwn::string::npos ) throw "bad face format 1";
					const std::size_t vertex = csizet(str.substr(0, a)) - 1;
					const std::size_t textureCoordiante = csizet(str.substr(a+1, b-a-1)) -1;
					const std::size_t normal = csizet(str.substr(b+1, str.length()-b-1)) -1;
					return mesh::Triangle::Vertex(vertex, normal, textureCoordiante);
				}

				const pwn::math::Rgba cRgba(const pwn::string& r, const pwn::string& g, const pwn::string& b, const pwn::real a)
				{
					return pwn::math::Rgba(creal(r), creal(g), creal(b), a);
				}
			}

			namespace // local
			{
				void Add(mesh::Mesh::MaterialPtr material, std::map<std::string, std::size_t>* materials, mesh::Mesh* converter)
				{
					if( material )
					{
						materials->insert( std::make_pair(material->name, converter->addMaterial(material)) );
					}
				}
				void LoadMaterialLibrary(mesh::Mesh* converter, std::map<std::string, std::size_t>* materials, const pwn::string& sourceFile, const pwn::string& materialLibraryFileName)
				{
					const pwn::string file = (boost::filesystem::path(sourceFile).remove_filename() / materialLibraryFileName).string();

					std::ifstream f(file.c_str());
					if( false == f.good() ) throw "missing file";

					mesh::Mesh::MaterialPtr material;

					pwn::string line;
					while( std::getline(f, line) )
					{
						const std::vector<pwn::string> sline = pwn::core::SplitString(pwn::core::Trim(line), pwn::core::kSpaceCharacters());
						const std::size_t slineSize = sline.size();
						if( slineSize == 0 ) continue; // empty line
						if( sline[0][0] == '#' ) continue; // comment
						const pwn::string command = sline[0];
						pwn::real transperency = 1;

						if( command == "newmtl" )
						{
							Add(material, materials, converter);
							material.reset(new mesh::Material());
							material->name = sline[1];
						}
						else if( command == "Ka")
						{
							// ambient r g b
							material->ambient = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "Kd")
						{
							// diffuse r g b
							material->diffuse = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "Ks")
						{
							// specular r g b
							material->specular = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "Ke")
						{
							// emissive r g b
							material->emission = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "d")
						{
							// transparency v
							transperency = creal(sline[1]);
							material->ambient.alpha(transperency);
							material->diffuse.alpha(transperency);
							material->specular.alpha(transperency);
							material->emission.alpha(transperency);
						}
						else if( command == "map_Kd")
						{
							// texture relative
							material->texture_diffuse = (boost::filesystem::path(sourceFile).remove_filename() / sline[1]).string();
						}
					}

					Add(material, materials, converter);
				}
			}

			pwn::string Read(std::ifstream& f)
			{
				pwn::string s;
				f >> s;
				return s;
			}

			void SkipWhitespace(std::ifstream& f)
			{
				while( f.peek() == ' ') f.ignore();
			}

			void read(OptimizedMeshBuilder* builder, const std::string& file, VoidVoidCallback& cb)
			{
				std::ifstream f(file.c_str());
				if( false == f.good() ) throw "missing obj file";

				std::map<std::string, std::size_t> materials;
				pwn::string currentMaterial = "";

				int lineIndex = 0;
				pwn::uint64 commandLine = 0;

				pwn::string command;
				while( f >> command )
				{
					++lineIndex;
					++commandLine;
					if( lineIndex > 9000 )
					{
						cb.perform();
						lineIndex = 0;
					}
					if( command[0] == '#' )
					{
						while( f.get() != '\n' );
						continue;
					}
					
					if( command == "v" )
					{
						pwn::string x = Read(f);
						pwn::string y = Read(f);
						pwn::string z = Read(f);
						builder->addPosition(pwn::math::vec3(creal(x), creal(y), creal(z)));
					}
					else if (command == "vt" )
					{
						pwn::string x = Read(f);
						pwn::string y = Read(f);
						builder->addTextCoord(pwn::math::vec2(creal(x), creal(y)));

						
						SkipWhitespace(f);
						char p = f.peek();
						if( pwn::string("0123456789").find_first_of(p) != pwn::string::npos )
						{
							Read(f); // optional w element ignored...
						}
					}
					else if ( command == "vn" )
					{
						// normalize, since the input may not be unit
						pwn::string x = Read(f);
						pwn::string y = Read(f);
						pwn::string z = Read(f);
						builder->addNormal(pwn::math::GetNormalized(pwn::math::vec3(creal(x), creal(y), creal(z))));
					}
					else if ( command == "f" )
					{
						std::vector<mesh::Triangle::Vertex> faces;
						SkipWhitespace(f);
						while( pwn::string("0123456789/").find(f.peek()) != pwn::string::npos )
						{
							faces.push_back(cFaceIndex(Read(f)));
							SkipWhitespace(f);
						}
						AddFace(builder->mesh(), materials[currentMaterial],faces);
					}
					else if ( command == "usemtl" )
					{
						currentMaterial = Read(f);
					}
					else if ( command == "mtllib" )
					{
						LoadMaterialLibrary(builder->mesh(), &materials, file, Read(f));
					}
					else if( command == "g" )
					{
						Read(f); // group-name
					}
					else if( command == "s" )
					{
						Read(f); // on/off?
					}
					else
					{
						pwn::string e = "unknown command: ";
						e+= command;
						throw e;
					}
				}

				builder->done();
			}
		}
	}
}