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

		class File
		{
		public:
			explicit File(const string& file)
				: f (file.c_str(), std::ios::out | std::ios::binary)
			{
				if( !f.good() ) throw "failed to open file for writing";
				f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );
			}

			typedef char Byte;
			void handle(pwn::uint16& i)
			{
				f.write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint16));
			}
			void handle(pwn::uint32& i)
			{
				f.write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint32));
			}
			void handle(pwn::real& r)
			{
				f.write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::real));
			}
			void write_sizet(std::size_t& s)
			{
				handle(static_cast<pwn::uint32>(s));
			}

			bool isLoading() const
			{
				return false;
			}
		private:
			std::ofstream f;
		};

		void Handle(File& f, pwn::real& r, bool compress)
		{
			if( compress )
			{
				pwn::uint16 half = pwn::math::FloatToHalf(r);
				f.handle(half);
				if( f.isLoading() )
				{
					r = pwn::math::HalfToFloat(half);
				}
			}
			else
			{
				f.handle(r);
			}
		}
		void Handle(File& f, pwn::math::Rgba& c, bool compress)
		{
			for(int i = 0; i<4; ++i)
			{
				Handle(f, c[i], compress);
			}
		}
		void Handle(File& f, pwn::math::vec3& v)
		{
			f.handle(v.x);
			f.handle(v.y);
			f.handle(v.z);
		}
		void WriteLocation(File& f, pwn::math::vec3& v, bool compress)
		{
			Handle(f, v.x, compress);
			Handle(f, v.y, compress);
			Handle(f, v.z, compress);
		}
		void WriteNormal(File& f, pwn::math::vec3& n, bool compress)
		{
			if( compress )
			{
				pwn::uint16 uv = pwn::math::UnitVectorToCompressed(n);
				f.handle(uv);
				if( f.isLoading() )
				{
					n = pwn::math::CompressedToUnitVector(uv);
				}
			}
			else
			{
				Handle(f, n);
			}
		}
		void Handle(File& f, pwn::math::vec2& v, bool compress)
		{
			Handle(f, v.x, compress);
			Handle(f, v.y, compress);
		}
		template<typename T>
		std::size_t HandleSize(File& f, std::vector<T>& vec)
		{
			std::size_t vc = vec.size();
			f.write_sizet(vc);
			if( f.isLoading() )
			{
				vec.resize(vc);
			}
			return vc;
		}

		void Write(mesh::Mesh& mesh, const pwn::string& file, const Compress compress)
		{
			File f(file);

			/* header */ {
				pwn::uint16 version = 0;
				f.handle(version);
				//pwn::uint16 flags = compress? 1 : 0;
				//f.handle( reinterpret_cast<const Byte*>(&flags), sizeof(pwn::uint16));
			}

			/* vertices */ {
				const std::size_t vc = HandleSize(f, mesh.positions);
				for(std::size_t i=0; i<vc; ++i)
				{
					WriteLocation(f, mesh.positions[i], compress.positions);
				}
			}

			/* texture coordinates */ {
				const std::size_t tc = HandleSize(f, mesh.texcoords);
				for(std::size_t i=0; i<tc; ++i)
				{
					Handle(f, mesh.texcoords[i], compress.texcoords);
				}
			}

			/* normals */ {
				const std::size_t nc = HandleSize(f, mesh.normals);
				for(std::size_t i=0; i<nc; ++i)
				{
					pwn::math::vec3& n = mesh.normals[i];
					WriteNormal(f, n, compress.normals);
				}
			}

			/* materials */ {
				const std::size_t mc = HandleSize(f, mesh.materials);
				for(std::size_t i=0; i<mc; ++i)
				{
					Handle(f, mesh.materials[i]->ambient, compress.materials);
					Handle(f, mesh.materials[i]->diffuse, compress.materials);
					Handle(f, mesh.materials[i]->specular, compress.materials);
					Handle(f, mesh.materials[i]->emission, compress.materials);
					Handle(f, mesh.materials[i]->shininess, compress.materials);
				}
			}

			/* faces */ {
				const std::size_t fc = HandleSize(f, mesh.triangles);
				for(std::size_t i=0; i<fc; ++i)
				{
					::pwn::mesh::Triangle& t = mesh.triangles[i];
					for(int faceIndex=0; faceIndex<3; ++faceIndex)
					{
						::pwn::mesh::Triangle::Vertex& v = t[faceIndex];
						f.handle(v.location);
						f.handle(v.normal);
						f.handle(v.texcoord);
					}
					f.handle(t.material);
				}
			}
		}
	}
}