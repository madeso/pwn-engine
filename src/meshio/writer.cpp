#include <pwn/meshio/io>

#include <fstream>
#include <iostream>

#include <pwn/mesh/mesh>
#include <pwn/math/operations>
#include <pwn/mesh/material>

namespace pwn
{
	namespace meshio
	{
		Compress::Compress(bool all)
			: materials(all)
			, positions(all)
			, normals(all)
			, texcoords(all)
		{
		}

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
			void Write(std::ofstream* f, const pwn::real& r, bool compress)
			{
				if( compress )
				{
					Write(f, pwn::math::FloatToHalf(r));
				}
				else f->write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::real));
			}
			void Write(std::ofstream* f, const pwn::math::Rgba& c, bool compress)
			{
				Write(f, c.red(), compress);
				Write(f, c.green(), compress);
				Write(f, c.blue(), compress);
				Write(f, c.alpha(), compress);
			}
			void Write(std::ofstream* f, const pwn::math::vec3& v, bool compress)
			{
				Write(f, v.x, compress);
				Write(f, v.y, compress);
				Write(f, v.z, compress);
			}
			void Write(std::ofstream* f, const pwn::math::vec2& v, bool compress)
			{
				Write(f, v.x, compress);
				Write(f, v.y, compress);
			}
		}

		void Write(const mesh::Mesh& mesh, const pwn::string& file, const Compress compress)
		{
			std::ofstream f(file.c_str(), std::ios::out | std::ios::binary);
			if( !f.good() ) throw "failed to open file for writing";
			f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );

			/* header */ {
				pwn::uint16 version = 0;
				f.write( reinterpret_cast<const Byte*>(&version), sizeof(pwn::uint16));
				//pwn::uint16 flags = compress? 1 : 0;
				//f.write( reinterpret_cast<const Byte*>(&flags), sizeof(pwn::uint16));
			}

			/* vertices */ {
				const std::size_t vc = mesh.positions.size();
				f.write( reinterpret_cast<const Byte*>(&vc), sizeof(std::size_t));
				for(std::size_t i=0; i<vc; ++i)
				{
					Write(&f, mesh.positions[i], compress.positions);
				}
			}

			/* texture coordinates */ {
				const std::size_t tc = mesh.texcoords.size();
				f.write(reinterpret_cast<const Byte*>(&tc), sizeof(std::size_t));
				for(std::size_t i=0; i<tc; ++i)
				{
					Write(&f, mesh.texcoords[i], compress.texcoords);
				}
			}

			/* normals */ {
				const std::size_t nc = mesh.normals.size();
				f.write(reinterpret_cast<const Byte*>(&nc), sizeof(std::size_t));
				for(std::size_t i=0; i<nc; ++i)
				{
					const pwn::math::vec3& n = mesh.normals[i];
					if( compress.normals )
					{
						Write(&f, pwn::math::UnitVectorToCompressed(n));
					}
					else
					{
						Write(&f, n, false);
					}
				}
			}

			/* materials */ {
				const std::size_t mc = mesh.materials.size();
				f.write(reinterpret_cast<const Byte*>(&mc), sizeof(std::size_t));
				for(std::size_t i=0; i<mc; ++i)
				{
					Write(&f, mesh.materials[i]->ambient, compress.materials);
					Write(&f, mesh.materials[i]->diffuse, compress.materials);
					Write(&f, mesh.materials[i]->specular, compress.materials);
					Write(&f, mesh.materials[i]->emission, compress.materials);
					Write(&f, mesh.materials[i]->shininess, compress.materials);
				}
			}

			/* faces */ {
				const std::size_t fc = mesh.triangles.size();
				f.write(reinterpret_cast<const Byte*>(&fc), sizeof(std::size_t));
				for(std::size_t i=0; i<fc; ++i)
				{
					const ::pwn::mesh::Triangle& t = mesh.triangles[i];
					for(int faceIndex=0; faceIndex<3; ++faceIndex)
					{
						const ::pwn::mesh::Triangle::Vertex& v = t[faceIndex];
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