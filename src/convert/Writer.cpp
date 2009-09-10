#include "Writer.hpp"

#include <fstream>
#include "Converter.hpp"

namespace pwn
{
	namespace convert
	{
		namespace // local
		{
			typedef char Byte;

			void Write(std::ofstream* f, const pwn::math::Rgba& c)
			{
				// todo: remove hacky assumption of that the r is the first one
				f->write(reinterpret_cast<const Byte*>(&c.r), sizeof(pwn::math::real)*4);
			}
		}

		void Write(Converter& data, const pwn::core::string& file)
		{
			std::ofstream f(file.c_str(), std::ios::out | std::ios::binary);
			if( !f.good() ) throw "failed to open file for writing";
			f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );

			/* vertices */ {
				const std::size_t vc = data.vertices.size();
				f.write( reinterpret_cast<const Byte*>(&vc), sizeof(std::size_t));
				for(std::size_t i=0; i<vc; ++i)
				{
					f.write(reinterpret_cast<const Byte*>(data.vertices[i].data()), sizeof(pwn::math::real) * 3 );
				}
			}

			/* texture coordinates */ {
				const std::size_t tc = data.textureCoordinates.size();
				f.write(reinterpret_cast<const Byte*>(&tc), sizeof(std::size_t));
				for(std::size_t i=0; i<tc; ++i)
				{
					f.write(reinterpret_cast<const Byte*>(data.textureCoordinates[i].data()), sizeof(pwn::math::real) * 2 );
				}
			}

			/* normals */ {
				const std::size_t nc = data.normals.size();
				f.write(reinterpret_cast<const Byte*>(&nc), sizeof(std::size_t));
				for(std::size_t i=0; i<nc; ++i)
				{
					f.write(reinterpret_cast<const Byte*>(data.normals[i].data()), sizeof(pwn::math::real) * 2 );
				}
			}

			/* materials */ {
				const std::size_t mc = data.materials.size();
				f.write(reinterpret_cast<const Byte*>(&mc), sizeof(std::size_t));
				for(std::size_t i=0; i<mc; ++i)
				{
					Write(&f, data.materials[i].ambient);
					Write(&f, data.materials[i].diffuse);
					Write(&f, data.materials[i].specular);
					Write(&f, data.materials[i].emissive);
					f.write(reinterpret_cast<const Byte*>(&data.materials[i].shininess), sizeof(pwn::math::real));
				}
			}

			/* faces */ {
				const std::size_t fc = data.faces.size();
				f.write(reinterpret_cast<const Byte*>(&fc), sizeof(std::size_t));
				for(std::size_t i=0; i<fc; ++i)
				{
					for(int faceIndex=0; faceIndex<3; ++faceIndex)
					{
						// todo: remove hacky assumption of that the vertex is the first one
						f.write(reinterpret_cast<const Byte*>(&data.faces[i].indices[faceIndex].vertex), sizeof(std::size_t) * 3);
					}
					f.write(reinterpret_cast<const Byte*>(&data.faces[i].material), sizeof(std::size_t));
				}
			}
		}
	}
}