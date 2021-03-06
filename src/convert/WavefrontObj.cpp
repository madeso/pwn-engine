#include <fstream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "WavefrontObj.hpp"

#include <pwn/core/stringutils.h>
#include <pwn/math/operations.h>

#include <pwn/mesh/builder.h>

#include <pwn/mesh/mesh.h>
#include <pwn/core/str.h>

#include <iostream>
#include <pwn/assert.h>

namespace pwn
{
namespace convert
{
    namespace obj
    {
        namespace  // local
        {
            std::size_t
            csizet(const pwn::string& str)
            {
                return boost::lexical_cast<std::size_t>(str);
            }
            long
            clong(const pwn::string& str)
            {
                return boost::lexical_cast<long>(str);
            }
            pwn::real
            creal(const pwn::string& str)
            {
                return boost::lexical_cast<pwn::real>(str);
            }

            std::size_t
            FixFaceIndex(long index, std::size_t count)
            {
                const std::size_t ret =
                        (index <= 0) ? count + index : index - 1;
                Assert(ret >= 0);
                Assert(count == 0 || ret < count);
                return ret;
            }

            const mesh::BTriangle::Vertex
            cFaceIndex(
                    const pwn::string& astr,
                    std::size_t pointCount,
                    std::size_t normalCount,
                    std::size_t uvCount)
            {
                const pwn::string str = pwn::core::Trim(astr);
                const pwn::string::size_type a = str.find_first_of('/');
                if (a == pwn::string::npos)
                {
                    throw "bad face format 1";
                }
                const pwn::string::size_type b = str.find_first_of('/', a + 1);
                //if( b == pwn::string::npos ) throw "bad face format 1";
                const long vertex = clong(str.substr(0, a));
                const long textureCoordiante = (b == pwn::string::npos)
                        ? clong(str.substr(a + 1))
                        : clong(str.substr(a + 1, b - a - 1));
                const long normal = (b == pwn::string::npos)
                        ? 1
                        : clong(str.substr(b + 1, str.length() - b - 1));
                return mesh::BTriangle::Vertex(
                        FixFaceIndex(vertex, pointCount),
                        FixFaceIndex(normal, normalCount),
                        FixFaceIndex(textureCoordiante, uvCount));
            }

            const pwn::math::Rgba
            cRgba(const pwn::string& r,
                  const pwn::string& g,
                  const pwn::string& b,
                  const pwn::real a)
            {
                return pwn::math::Rgba(creal(r), creal(g), creal(b), a);
            }
        }

        namespace  // local
        {
            void
            LoadMaterialLibrary(
                    mesh::Builder* builder,
                    const pwn::string& sourceFile,
                    const pwn::string& materialLibraryFileName)
            {
                const pwn::string file =
                        (boost::filesystem::path(sourceFile).remove_filename() /
                         materialLibraryFileName)
                                .string();

                std::ifstream f(file.c_str());
                if (false == f.good())
                {
                    throw "missing file";
                }

                mesh::Material material;
                pwn::string materialname;

                pwn::string line;
                while (std::getline(f, line))
                {
                    const std::vector<pwn::string> sline =
                            pwn::core::SplitString(
                                    pwn::core::Trim(line),
                                    pwn::core::kSpaceCharacters());
                    const std::size_t slineSize = sline.size();
                    if (slineSize == 0)
                    {
                        continue;  // empty line
                    }
                    if (sline[0][0] == '#')
                    {
                        continue;  // comment
                    }
                    const pwn::string command = sline[0];
                    pwn::real transperency = 1;

                    if (command == "newmtl")
                    {
                        builder->addMaterial(materialname, material);
                        material = mesh::Material();
                        materialname = sline[1];
                    }
                    else if (command == "Ka")
                    {
                        // ambient r g b
                        material.ambient =
                                cRgba(sline[1],
                                      sline[2],
                                      sline[3],
                                      transperency);
                    }
                    else if (command == "Kd")
                    {
                        // diffuse r g b
                        material.diffuse =
                                cRgba(sline[1],
                                      sline[2],
                                      sline[3],
                                      transperency);
                    }
                    else if (command == "Ks")
                    {
                        // specular r g b
                        material.specular =
                                cRgba(sline[1],
                                      sline[2],
                                      sline[3],
                                      transperency);
                    }
                    else if (command == "Ke")
                    {
                        // emissive r g b
                        material.emission =
                                cRgba(sline[1],
                                      sline[2],
                                      sline[3],
                                      transperency);
                    }
                    else if (command == "d")
                    {
                        // transparency v
                        transperency = creal(sline[1]);
                        material.ambient.alpha(transperency);
                        material.diffuse.alpha(transperency);
                        material.specular.alpha(transperency);
                        material.emission.alpha(transperency);
                    }
                    else if (command == "map_Kd")
                    {
                        // diffuse texture
                        material.setTexture_Diffuse(
                                (boost::filesystem::path(sourceFile)
                                         .remove_filename() /
                                 sline[1])
                                        .string());
                    }
                    else if (command == "map_Ka")
                    {
                        // texture ambient
                    }
                    else if (command == "map_Ks")
                    {
                        // texture specular
                    }
                    else if (command == "map_Ks")
                    {
                        // texture specular
                    }
                    else if (command == "Ns")
                    {
                        // specular coefficient
                    }
                    else if (command == "illum")
                    {
                        // illumination mode
                    }
                    else
                    {
                        throw(core::Str()
                              << "Unknown material command " << command)
                                .toString();
                    }
                }

                builder->addMaterial(materialname, material);
            }
        }

        pwn::string
        Read(std::ifstream& f)
        {
            pwn::string s;
            f >> s;
            return s;
        }

        void
        SkipWhitespace(std::ifstream& f)
        {
            while (f.peek() == ' ')
            {
                f.ignore();
            }
        }

        void
        read(BuilderList* builders,
             const std::string& file,
             VoidVoidCallback& cb)
        {
            std::ifstream f(file.c_str());
            if (false == f.good())
            {
                throw "missing obj file";
            }

            mesh::Builder builder;

            pwn::string currentMaterial = "";

            int lineIndex = 0;
            pwn::uint64 commandLine = 0;

            pwn::string command;
            while (f >> command)
            {
                ++lineIndex;
                ++commandLine;
                if (lineIndex > 9000)
                {
                    cb.perform();
                    lineIndex = 0;
                }
                if (command[0] == '#')
                {
                    while (f.get() != '\n')
                        ;
                    continue;
                }

                if (command == "v")
                {
                    pwn::string x = Read(f);
                    pwn::string y = Read(f);
                    pwn::string z = Read(f);
                    builder.addPosition(pwn::mesh::BPoint(
                            pwn::math::vec3(creal(x), creal(y), creal(z)),
                            math::vec4(-1, -1, -1, -1)));
                }
                else if (command == "vt")
                {
                    pwn::string x = Read(f);
                    pwn::string y = Read(f);
                    //Assert(0 && "do we need to add a 1-y when adding a textcoord?");
                    builder.addTextCoord(pwn::math::vec2(creal(x), creal(y)));


                    SkipWhitespace(f);
                    char p = f.peek();
                    if (pwn::string("0123456789").find_first_of(p) !=
                        pwn::string::npos)
                    {
                        Read(f);  // optional w element ignored...
                    }
                }
                else if (command == "vn")
                {
                    // normalize, since the input may not be unit
                    pwn::string x = Read(f);
                    pwn::string y = Read(f);
                    pwn::string z = Read(f);
                    builder.addNormal(pwn::math::GetNormalized(
                            pwn::math::vec3(creal(x), creal(y), creal(z))));
                }
                else if (command == "f")
                {
                    std::vector<mesh::BTriangle::Vertex> faces;
                    SkipWhitespace(f);
                    while (pwn::string("-0123456789/").find(f.peek()) !=
                           pwn::string::npos)
                    {
                        faces.push_back(cFaceIndex(
                                Read(f),
                                builder.positions.size(),
                                builder.normals.size(),
                                builder.texcoords.size()));
                        SkipWhitespace(f);
                    }
                    builder.addFace(
                            builder.getMaterial(currentMaterial),
                            faces);
                }
                else if (command == "usemtl")
                {
                    currentMaterial = Read(f);
                }
                else if (command == "mtllib")
                {
                    LoadMaterialLibrary(&builder, file, Read(f));
                }
                else if (command == "g")
                {
                    pwn::string name;
                    std::getline(f, name);
                    //Read(f); // group-name
                }
                else if (command == "s")
                {
                    Read(f);  // on/off?
                }
                else
                {
                    pwn::string e = "unknown command: ";
                    e += command;
                    throw e;
                }
            }

            builders->push_back(builder);
        }
    }
}
}
