#include <pwn/meshio/io>

#include <fstream>
#include <iostream>

#include <pwn/mesh/mesh>
#include <pwn/math/operations>
#include <pwn/mesh/material>
#include <boost/scoped_array.hpp>

#include <physfs.h>

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

		static void Error(const pwn::string& message)
		{
			const pwn::string vfsError = PHYSFS_getLastError();
			throw "Unable to " + message + ": " + vfsError;
		}

		WriteTarget::WriteTarget(const pwn::string& target)
		{
			if( 0 == PHYSFS_init(NULL) ) Error("init");
			set(target);
		}

		WriteTarget::~WriteTarget()
		{
			PHYSFS_deinit();
		}

		void WriteTarget::set(const pwn::string& target)
		{
			if( 0 == PHYSFS_setWriteDir(target.c_str()) ) Error("change write dir");
		}

		class VirtualFile
		{
		public:
			VirtualFile(const pwn::string& path, bool isLoading)
				: file(isLoading?PHYSFS_openRead(path.c_str()):PHYSFS_openWrite(path.c_str()))
			{
				if( file != 0 ) return;
				const pwn::string action = isLoading?"read":"write";
				Error(action + " " + path);
			}

			~VirtualFile()
			{
				if( file ) PHYSFS_close(file);
			}

			void handle8(const pwn::uint8& j)
			{
				const pwn::uint16 i = j;
				if( 0 == PHYSFS_writeULE16(file, i) ) Error("write uint8");
			}
			void handle8(pwn::uint8& j)
			{
				pwn::uint16 i=0;
				if( 0 == PHYSFS_readULE16(file, &i) ) Error("read uint8");
				j = static_cast<pwn::uint8>(i);
			}

			void handle16(const pwn::uint16& i)
			{
				if( 0 == PHYSFS_writeULE16(file, i) ) Error("write uint16");
			}
			void handle16(pwn::uint16& i)
			{
				if( 0 == PHYSFS_readULE16(file, &i) ) Error("read uint16");
			}

			void write32(const pwn::uint32& i)
			{
				if( 0 == PHYSFS_writeULE32(file, i) ) Error("write uint32");
			}

			void read32(pwn::uint32& i)
			{
				if( 0 == PHYSFS_readULE32(file, &i) ) Error("read uint32");
			}

			void handle32(const pwn::uint32& i)
			{
				write32(i);
			}
			void handle32(pwn::uint32& i)
			{
				read32(i);
			}

			void handleReal(const pwn::real& r)
			{
				if( PHYSFS_write(file, &r, 1, sizeof(pwn::real)) != sizeof(pwn::real) ) Error("write real");
			}
			void handleReal(pwn::real& r)
			{
				if( PHYSFS_read(file, &r, 1, sizeof(pwn::real)) != sizeof(pwn::real) ) Error("reading real");
			}

			void handleString(const pwn::string& r)
			{
				const uint32 length = r.length();
				write32(length);
				if( PHYSFS_write(file, r.c_str(), 1, length*sizeof(pwn::tchar)) != length*sizeof(pwn::tchar) ) Error("write string");
			}
			void handleString(pwn::string& r)
			{
				uint32 length = 0;
				read32(length);
				if( length == 0 )
				{
					r = "";
				}
				else
				{
					boost::scoped_array<pwn::tchar> arr( new tchar[length+1] );
					if( PHYSFS_read(file, arr.get(), 1, length * sizeof(pwn::tchar)) != length*sizeof(pwn::tchar) ) Error("reading string");
					arr[length] = 0;
					r = arr.get();
				}
			}

			/*void handle32(const pwn::real r)
			{
				const pwn::uint32 i = (pwn::uint32) r;
				handle32(i);
			}
			void handle32(pwn::real& r)
			{
				pwn::uint32 i = (pwn::uint32) r;
				handle32(i);
				r = i;
			}*/

			PHYSFS_file* file;
		};

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

		void Reset(const pwn::mesh::Mesh::MaterialPtr&)
		{
		}
		void Reset(pwn::mesh::Mesh::MaterialPtr& m)
		{
			m.reset( new mesh::Material() );
		}

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
			ARG(pwn::mesh::Material) Materiala;

			template<typename T>
			struct vectora
			{
				ARG(std::vector<T>) Type;
			};
#undef ARG

			FileArchiver(const pwn::string& filename)
				: vf( filename, IsLoading )
			{
			}
		private:
			VirtualFile vf;
			// util function
			typedef char Byte;
			void handle(uint8a i)
			{
				vf.handle8(i);
			}
			void handle(uint16a i)
			{
				vf.handle16(i);
			}
			void handle(uint32a i)
			{
				vf.handle32(i);
			}
			void handle(reala r)
			{
				vf.handleReal(r);
			}
			void write_sizet(sizeta s)
			{
				vf.handle32(s);
			}

			void handleCompressedNormal(const pwn::math::vec3& n)
			{
				const pwn::uint16 uv = pwn::math::UnitVectorToCompressed(n);
				handle(uv);
			}

			void handleCompressedNormal(pwn::math::vec3& n)
			{
				pwn::uint16 uv = 0;
				handle(uv);
				n = pwn::math::CompressedToUnitVector(uv);
			}

			void handleString(const pwn::string& n)
			{
				vf.handleString(n);
			}

			void handleString(pwn::string& n)
			{
				vf.handleString(n);
			}

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
			void handleLocation(vec3a v, bool compress)
			{
				handle(v.x, compress);
				handle(v.y, compress);
				handle(v.z, compress);
			}
			void handleNormal(vec3a n, bool compress)
			{
				if( compress )
				{
					handleCompressedNormal(n);
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
						handleLocation(mesh.positions[i], compress.positions);
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
						handleNormal(n, compress.normals);
					}
				}

				/* materials */ {
					const std::size_t mc = handleSize<pwn::mesh::Mesh::MaterialPtr>(mesh.materials);
					for(std::size_t i=0; i<mc; ++i)
					{
						Reset(mesh.materials[i]);
						Materiala m = *mesh.materials[i].get();
						handle(m.ambient, compress.materials);
						handle(m.diffuse, compress.materials);
						handle(m.specular, compress.materials);
						handle(m.emission, compress.materials);
						handle(m.shininess, compress.materials);
						handleString(m.texture_diffuse);
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

		namespace // local
		{
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
#define READ(n) c.n = (u&(1<<offsets::n)) != 0
				READ(materials);
				READ(positions);
				READ(normals);
				READ(texcoords);
#undef READ
				return c;
			}
		}

		const pwn::uint8 kVersion = 0;

		void Write(const mesh::Mesh& mesh, const pwn::string& filename, const pwn::meshio::Compress compress)
		{
			pwn::uint8 version = kVersion;
			FileArchiver<false> fa(filename);
			fa.handle(mesh, compress, version);
		}

		void Read(mesh::Mesh* mesh, const pwn::string& filename)
		{
			pwn::meshio::Compress compress(false);
			pwn::uint8 version = kVersion;
			FileArchiver<true> fa(filename);
			fa.handle(*mesh, compress, version);
		}
	}
}