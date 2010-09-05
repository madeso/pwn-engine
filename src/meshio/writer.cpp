#include <pwn/meshio/io.h>

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include <pwn/mesh/material.h>
#include "vfs.h"

namespace pwn
{
	namespace meshio
	{
		const pwn::uint8 kVersion = 0;

		class MeshWriter
		{
		public:
			mesh::Mesh* mesh;
		};

		template<class MeshArg>
		class MeshFile
		{
		public:
			template<class T>
			static void handleVector(VirtualFile& vf, const pwn::std::vector<T>& vector, bool compress)
			{
				pwn::uint32 size = vector.size();
				vf.write32(size);
				vf.write(vector.get(), size * sizeof(T));
			}

			template<class T>
			static void handleVector(VirtualFile& vf, pwn::std::vector<T>& vector, bool compress)
			{
				pwn::uint32 size = vf.read32();
				vector.reset(size);
				vf.read(vector.get(), size * sizeof(T));
			}

			static void handle(VirtualFile& vf, MeshArg mesh, Compress& c, pwn::uint8& version)
			{
				vf.handle8(version);
				if( version != kVersion ) throw "error";
				handleVector(vf, mesh.positions, c.positions);
				handleVector(vf, mesh.normals, c.normals);
				handleVector(vf, mesh.texcoords, c.texcoords);
				handleVector(vf, mesh.bones, c.texcoords);
			}
		};

		void Write(const mesh::Mesh& mesh, const pwn::string& filename, const Compress compress)
		{
			Compress c = compress;
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, false);
			MeshFile<const mesh::Mesh&>::handle(vf, mesh, c, version);
		}

		void Read(mesh::Mesh* mesh, const pwn::string& filename)
		{
			Compress compress(false);
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, true);
			MeshWriter writer;
			writer.mesh = mesh;
			MeshFile<mesh::Mesh&>::handle(vf, *mesh, compress, version);
		}
	}
}