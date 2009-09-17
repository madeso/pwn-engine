#include "Writer.hpp"

#include <fstream>
#include "Converter.hpp"
#include <pwn/math/operations>

namespace pwn
{
	namespace convert
	{
		namespace // local
		{
			typedef char Byte;

			const pwn::math::vec2 cvec2(const math::vec3& v)
			{
				return pwn::math::vec2(v.x, v.y);
			}

			void Write(std::ofstream* f, const pwn::math::real& r, bool optimize)
			{
				if( optimize )
				{
					pwn::math::uint16 i = pwn::math::FloatToHalf(r);
					f->write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::math::uint16));
				}
				else f->write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::math::real));
			}
			void Write(std::ofstream* f, const pwn::math::Rgba& c, bool optimize)
			{
				Write(f, c.r, optimize);
				Write(f, c.g, optimize);
				Write(f, c.b, optimize);
				Write(f, c.a, optimize);
			}
			void Write(std::ofstream* f, const pwn::math::vec3& v, bool optimize)
			{
				Write(f, v.x, optimize);
				Write(f, v.y, optimize);
				Write(f, v.z, optimize);
			}
			void Write(std::ofstream* f, const pwn::math::vec2& v, bool optimize)
			{
				Write(f, v.x, optimize);
				Write(f, v.y, optimize);
			}
		}

		void Write(Converter& data, const pwn::core::string& file, bool optimize)
		{
			std::ofstream f(file.c_str(), std::ios::out | std::ios::binary);
			if( !f.good() ) throw "failed to open file for writing";
			f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );

			/* header */ {
				pwn::math::uint16 version = 0;
				f.write( reinterpret_cast<const Byte*>(&version), sizeof(pwn::math::uint16));
				pwn::math::uint16 flags = optimize? 1 : 0;
				f.write( reinterpret_cast<const Byte*>(&flags), sizeof(pwn::math::uint16));
			}

			/* vertices */ {
				const std::size_t vc = data.vertices.size();
				f.write( reinterpret_cast<const Byte*>(&vc), sizeof(std::size_t));
				for(std::size_t i=0; i<vc; ++i)
				{
					Write(&f, data.vertices[i], optimize);
				}
			}

			/* texture coordinates */ {
				const std::size_t tc = data.textureCoordinates.size();
				f.write(reinterpret_cast<const Byte*>(&tc), sizeof(std::size_t));
				for(std::size_t i=0; i<tc; ++i)
				{
					Write(&f, data.textureCoordinates[i], optimize);
				}
			}

			/* normals */ {
				const std::size_t nc = data.normals.size();
				f.write(reinterpret_cast<const Byte*>(&nc), sizeof(std::size_t));
				for(std::size_t i=0; i<nc; ++i)
				{
					Write(&f, cvec2(data.normals[i]), optimize);
				}
			}

			/* materials */ {
				const std::size_t mc = data.materials.size();
				f.write(reinterpret_cast<const Byte*>(&mc), sizeof(std::size_t));
				for(std::size_t i=0; i<mc; ++i)
				{
					Write(&f, data.materials[i].ambient, optimize);
					Write(&f, data.materials[i].diffuse, optimize);
					Write(&f, data.materials[i].specular, optimize);
					Write(&f, data.materials[i].emissive, optimize);
					Write(&f, data.materials[i].shininess, optimize);
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