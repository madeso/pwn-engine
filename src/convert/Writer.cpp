#include "Writer.hpp"

#include <fstream>
#include <pwn/math/operations>
#include <pwn/mesh/material>

#include <iostream>

#include <pwn/mesh/Mesh>

namespace pwn
{
	namespace convert
	{
		namespace // local
		{
			typedef char Byte;

			void Write(std::ofstream* f, const pwn::uint16 i)
			{
				f->write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint16));
			}
			void Write(std::ofstream* f, const pwn::uint32 i)
			{
				f->write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint32));
			}
			void Write(std::ofstream* f, const pwn::real& r, bool optimize)
			{
				if( optimize )
				{
					Write(f, pwn::math::FloatToHalf(r));
				}
				else f->write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::real));
			}
			void Write(std::ofstream* f, const pwn::math::Rgba& c, bool optimize)
			{
				Write(f, c.red(), optimize);
				Write(f, c.green(), optimize);
				Write(f, c.blue(), optimize);
				Write(f, c.alpha(), optimize);
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

		void Write(mesh::Mesh& mesh, const pwn::string& file, bool optimize)
		{
			std::ofstream f(file.c_str(), std::ios::out | std::ios::binary);
			if( !f.good() ) throw "failed to open file for writing";
			f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );

			/* header */ {
				pwn::uint16 version = 0;
				f.write( reinterpret_cast<const Byte*>(&version), sizeof(pwn::uint16));
				pwn::uint16 flags = optimize? 1 : 0;
				f.write( reinterpret_cast<const Byte*>(&flags), sizeof(pwn::uint16));
			}

			/* vertices */ {
				const std::size_t vc = mesh.positions.size();
				f.write( reinterpret_cast<const Byte*>(&vc), sizeof(std::size_t));
				for(std::size_t i=0; i<vc; ++i)
				{
					Write(&f, mesh.positions[i], optimize);
				}
			}

			/* texture coordinates */ {
				const std::size_t tc = mesh.texcoords.size();
				f.write(reinterpret_cast<const Byte*>(&tc), sizeof(std::size_t));
				for(std::size_t i=0; i<tc; ++i)
				{
					Write(&f, mesh.texcoords[i], optimize);
				}
			}

			/* normals */ {
				const std::size_t nc = mesh.normals.size();
				f.write(reinterpret_cast<const Byte*>(&nc), sizeof(std::size_t));
				for(std::size_t i=0; i<nc; ++i)
				{
					Write(&f, pwn::math::UnitVectorToCompressed(mesh.normals[i]));
				}
			}

			/* materials */ {
				const std::size_t mc = mesh.materials.size();
				f.write(reinterpret_cast<const Byte*>(&mc), sizeof(std::size_t));
				for(std::size_t i=0; i<mc; ++i)
				{
					Write(&f, mesh.materials[i]->ambient, optimize);
					Write(&f, mesh.materials[i]->diffuse, optimize);
					Write(&f, mesh.materials[i]->specular, optimize);
					Write(&f, mesh.materials[i]->emission, optimize);
					Write(&f, mesh.materials[i]->shininess, optimize);
				}
			}

			/* faces */ {
				const std::size_t fc = mesh.triangles.size();
				f.write(reinterpret_cast<const Byte*>(&fc), sizeof(std::size_t));
				for(std::size_t i=0; i<fc; ++i)
				{
					::pwn::mesh::Triangle& t = mesh.triangles[i];
					for(int faceIndex=0; faceIndex<3; ++faceIndex)
					{
						::pwn::mesh::Triangle::Vertex& v = t[faceIndex];
						Write(&f, v.location);
						Write(&f, v.normal);
						Write(&f, v.texcoord);
					}
					f.write(reinterpret_cast<const Byte*>(&t.material), sizeof(std::size_t));
				}
			}
		}
	}
}