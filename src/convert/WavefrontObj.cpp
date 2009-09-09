#include <fstream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "WavefrontObj.hpp"
#include "Converter.hpp"

#include <pwn/core/StringUtils>
#include <pwn/math/operations>

namespace pwn
{
	namespace convert
	{
		namespace obj
		{
			namespace // local
			{
				std::size_t csizet(const pwn::core::string& str)
				{
					return boost::lexical_cast<std::size_t>(str);
				}
				pwn::math::real creal(const pwn::core::string& str)
				{
					return boost::lexical_cast<pwn::math::real>(str);
				}

				const FaceIndex cFaceIndex(const pwn::core::string& astr)
				{
					const pwn::core::string str = pwn::core::Trim(astr);
					const pwn::core::string::size_type a = str.find_first_of('/');
					if( a == pwn::core::string::npos ) throw "bad face format 1";
					const pwn::core::string::size_type b = str.find_first_of('/', a+1);
					if( b == pwn::core::string::npos ) throw "bad face format 1";
					FaceIndex f;
					f.vertex = csizet(str.substr(0, a)) - 1;
					f.textureCoordiante = csizet(str.substr(a+1, b-a-1)) -1;
					f.normal = csizet(str.substr(b+1, str.length()-b-1)) -1;
					return f;
				}

				const pwn::math::Rgba cRgba(const pwn::core::string& r, const pwn::core::string& g, const pwn::core::string& b, const pwn::math::real a)
				{
					return pwn::math::Rgba(creal(r), creal(g), creal(b), a);
				}
			}

			namespace // local
			{
				void AddIfValid(const Material& mat, Converter* converter, std::map<std::string, std::size_t>* materials)
				{
					if( mat.name == "" ) return;
					materials->insert( std::make_pair(mat.name, converter->addMaterial(mat)) );
				}

				void LoadMaterialLibrary(Converter* converter, std::map<std::string, std::size_t>* materials, const pwn::core::string& sourceFile, const pwn::core::string& materialLibraryFileName)
				{
					const pwn::core::string file = (boost::filesystem::path(sourceFile).remove_filename() / materialLibraryFileName).string();

					std::ifstream f(file.c_str());
					if( false == f.good() ) throw "missing file";

					Material material;

					pwn::core::string line;
					while( std::getline(f, line) )
					{
						const std::vector<pwn::core::string> sline = pwn::core::SplitString(pwn::core::Trim(line), pwn::core::kSpaceCharacters());
						const std::size_t slineSize = sline.size();
						if( slineSize == 0 ) continue; // empty line
						if( sline[0][0] == '#' ) continue; // comment
						const pwn::core::string command = sline[0];
						pwn::math::real transperency = 1;

						if( command == "newmtl" )
						{
							AddIfValid(material, converter, materials);
							material = Material();
							material.name = sline[1];
						}
						else if( command == "Ka")
						{
							// ambient r g b
							material.ambient = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "Kd")
						{
							// diffuse r g b
							material.diffuse = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "Ks")
						{
							// specular r g b
							material.specular = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "Ke")
						{
							// emissive r g b
							material.emissive = cRgba(sline[1], sline[2], sline[3], transperency);
						}
						else if( command == "d")
						{
							// transparency v
							transperency = creal(sline[1]);
							material.ambient.a = transperency;
							material.diffuse.a = transperency;
							material.specular.a = transperency;
							material.emissive.a = transperency;
						}
						else if( command == "map_Kd")
						{
							// texture relative
							material.textureDiffuse = (boost::filesystem::path(sourceFile).remove_filename() / sline[1]).string();
						}
					}

					AddIfValid(material, converter, materials);
				}
			}

			void read(Converter* converter, const std::string& file)
			{
				std::ifstream f(file.c_str());
				if( false == f.good() ) throw "missing obj file";

				std::map<std::string, std::size_t> materials;
				pwn::core::string currentMaterial = "";

				pwn::core::string line;
				while( std::getline(f, line) )
				{
					const std::vector<pwn::core::string> sline = pwn::core::SplitString(pwn::core::Trim(line), pwn::core::kSpaceCharacters());
					const std::size_t slineSize = sline.size();
					if( slineSize == 0 ) continue; // empty line
					if( sline[0][0] == '#' ) continue; // comment
					const pwn::core::string command = sline[0];
					
					if( command == "v" )
					{
						converter->addVertex(pwn::math::vec3(creal(sline[1]), creal(sline[2]), creal(sline[3])));
					}
					else if (command == "vt" )
					{
						converter->addTextureCoordinate(pwn::math::vec2(creal(sline[1]), creal(sline[2])));
					}
					else if ( command == "vn" )
					{
						// normalize, since the input may not be unit
						converter->addNormal(pwn::math::GetNormalized(pwn::math::vec3(creal(sline[1]), creal(sline[2]), creal(sline[3]))));
					}
					else if ( command == "f" )
					{
						std::vector<FaceIndex> faces;
						for(std::size_t i = 1; i<slineSize; ++i)
						{
							faces.push_back(cFaceIndex(sline[i]));
						}
						converter->addFace(materials[currentMaterial],faces);
					}
					else if ( command == "usemtl" )
					{
						currentMaterial = sline[1];
					}
					else if ( command == "mtllib" )
					{
						LoadMaterialLibrary(converter, &materials, file, sline[1]);
					}
				}
			}
		}
	}
}