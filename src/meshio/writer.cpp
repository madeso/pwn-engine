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

		template<bool CanModify, typename T>
		struct Argument {};

		template<typename T>
		struct Argument<true, T>
		{
			typedef T& Arg;
			static void Assign(Arg a, const T& t)
			{
				a = t;
			}

			template<typename T>
			static void Resize(std::vector<T>& vec, std::size_t vc)
			{
				vec.resize(vc);
			}
		};

		template<typename T>
		struct Argument<false, T>
		{
			typedef const T& Arg;
			static void Assign(Arg, Arg)
			{
			}

			template<typename T>
			static void Resize(const std::vector<T>&, std::size_t)
			{
			}
		};

		template <bool IsLoading>
		class FileArchiver
		{
		public:
#define ARG(T) typedef typename Argument<IsLoading, T>::Arg
			ARG(mesh::Mesh) MeshArg;
			ARG(Compress) CompressArg;

			ARG(pwn::uint8) uint8a;
			ARG(pwn::uint16) uint16a;
			ARG(pwn::uint32) uint32a;
			ARG(pwn::real) reala;
			ARG(std::size_t) sizeta;

			ARG(pwn::math::vec2) vec2a;
			ARG(pwn::math::vec3) vec3a;
			ARG(pwn::math::Rgba) Rgbaa;

			ARG(pwn::mesh::Triangle) Trianglea;
			ARG(pwn::mesh::Triangle::Vertex) Vertexa;

			template<typename T>
			struct vectora
			{
				typedef typename std::vector<T> Vec;
				typedef typename Argument<IsLoading, Vec>::Arg Type;
			};
#undef ARG

			FileArchiver(const pwn::string& filename)
				: f (filename.c_str(), std::ios::out | std::ios::binary)
			{
				if( !f.good() ) throw "failed to open file for writing";
				f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );
			}
		private:
			// util function
			typedef char Byte;
			void handle(uint8a i)
			{
				f.write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint8));
			}
			void handle(uint16a i)
			{
				f.write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint16));
			}
			void handle(uint32a i)
			{
				f.write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint32));
			}
			void handle(reala r)
			{
				f.write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::real));
			}
			void write_sizet(sizeta s)
			{
				handle(static_cast<pwn::uint32>(s));
			}
			std::ofstream f;

			/*template<typename T>
			void handleSize(std::vector<T>& vec, std::size_t vc)
			{
				vec.resize(vc);
			}*/

			template<typename T>
			std::size_t handleSize(typename vectora<T>::Type mvec)
			{
				std::size_t vc = mvec.size();
				write_sizet(vc);
				Argument<IsLoading, T>::Resize(mvec, vc);
				return vc;
			}

		private:
			// more util functions that use the 

			void handle(reala r, bool compress)
			{
				if( compress )
				{
					pwn::uint16 half = pwn::math::FloatToHalf(r);
					handle(half);
					Argument<IsLoading, pwn::real>::Assign(r, pwn::math::HalfToFloat(half));
				}
				else
				{
					handle(r);
				}
			}
			void handle(Rgbaa c, bool compress)
			{
				for(int i = 0; i<4; ++i)
				{
					handle(c[i], compress);
				}
			}
			void handle(vec3a v)
			{
				handle(v.x);
				handle(v.y);
				handle(v.z);
			}
			void WriteLocation(vec3a v, bool compress)
			{
				handle(v.x, compress);
				handle(v.y, compress);
				handle(v.z, compress);
			}
			void WriteNormal(vec3a n, bool compress)
			{
				if( compress )
				{
					pwn::uint16 uv = pwn::math::UnitVectorToCompressed(n);
					handle(uv);
					Argument<IsLoading, pwn::math::vec3>::Assign(n, pwn::math::CompressedToUnitVector(uv) );
				}
				else
				{
					handle(n);
				}
			}
			void handle(vec2a v, bool compress)
			{
				handle(v.x, compress);
				handle(v.y, compress);
			}

		public:
			// main function
			void handle(MeshArg mesh, CompressArg compress, uint8a version)
			{
				/* header */ {
					pwn::uint8 v = version;
					handle(v);
					Argument<IsLoading, pwn::uint8>::Assign(version, v);
					pwn::uint8 c = CompressArgsToUint8(compress);
					handle(c);

					Argument<IsLoading, Compress>::Assign(compress, Uint8ToCompressArgs(c));
				}

				/* vertices */ {
					const std::size_t vc = handleSize<pwn::math::vec3>(mesh.positions);
					for(std::size_t i=0; i<vc; ++i)
					{
						WriteLocation(mesh.positions[i], compress.positions);
					}
				}

				/* texture coordinates */ {
					const std::size_t tc = handleSize<pwn::math::vec2>(mesh.texcoords);
					for(std::size_t i=0; i<tc; ++i)
					{
						handle(mesh.texcoords[i], compress.texcoords);
					}
				}

				/* normals */ {
					const std::size_t nc = handleSize<pwn::math::vec3>(mesh.normals);
					for(std::size_t i=0; i<nc; ++i)
					{
						vec3a n = mesh.normals[i];
						WriteNormal(n, compress.normals);
					}
				}

				/* materials */ {
					const std::size_t mc = handleSize<pwn::mesh::Mesh::MaterialPtr>(mesh.materials);
					for(std::size_t i=0; i<mc; ++i)
					{
						handle(mesh.materials[i]->ambient, compress.materials);
						handle(mesh.materials[i]->diffuse, compress.materials);
						handle(mesh.materials[i]->specular, compress.materials);
						handle(mesh.materials[i]->emission, compress.materials);
						handle(mesh.materials[i]->shininess, compress.materials);
					}
				}

				/* faces */ {
					const std::size_t fc = handleSize<pwn::mesh::Triangle>(mesh.triangles);
					for(std::size_t i=0; i<fc; ++i)
					{
						Trianglea t = mesh.triangles[i];
						for(int faceIndex=0; faceIndex<3; ++faceIndex)
						{
							Vertexa v = t[faceIndex];
							handle(v.location);
							handle(v.normal);
							handle(v.texcoord);
						}
						handle(t.material);
					}
				}
			}
		};

		namespace offsets
		{
			enum 
			{
				materials,
				positions,
				normals,
				texcoords
			};
		}

		pwn::uint8 CompressArgsToUint8(const Compress& c)
		{
			uint8 res = 0;
#define WRITE(n) res |= (c.n?1:0) << offsets::n
			WRITE(materials);
			WRITE(positions);
			WRITE(normals);
			WRITE(texcoords);
#undef WRITE
			return res;
		}

		Compress Uint8ToCompressArgs(pwn::uint8 u)
		{
			Compress c(false);
#define READ(n) c.n = (u&offsets::n) != 0
			READ(materials);
			READ(positions);
			READ(normals);
			READ(texcoords);
#undef READ
			return c;
		}

		const pwn::uint8 kVersion = 0;

		void Write(const mesh::Mesh& mesh, const pwn::string& filename, const pwn::meshio::Compress compress)
		{
			pwn::uint8 version = kVersion;
			FileArchiver<false> fa(filename);
			fa.handle(mesh, compress, version);
		}

		void Read(mesh::Mesh* mesh, const pwn::string& filename, pwn::meshio::Compress* compress)
		{
			pwn::uint8 version = kVersion;
			FileArchiver<true> fa(filename);
			fa.handle(*mesh, *compress, version);
		}
	}
}