#include <pwn/meshio/io.h>

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include "vfs.h"

namespace pwn
{
	namespace meshio
	{
		const pwn::uint8 kVersion = 0;

		template<class MeshArg>
		class MeshFile
		{
		public:
			template<class T>
			static void handleVector(VirtualFile& vf, const std::vector<T>& vector)
			{
				pwn::uint32 size = vector.size();
				vf.write32(size);
				vf.write(&vector[0], size * sizeof(T));
			}

			template<class T>
			static void handleVector(VirtualFile& vf, std::vector<T>& vector)
			{
				pwn::uint32 size = vf.read32();
				vector.resize(size);
				vf.read(&vector[0], size * sizeof(T));
			}

			static void handle(VirtualFile& vf, MeshArg mesh, pwn::uint8& version)
			{
				vf.handle8(version);
				if( version != kVersion ) throw "error";
				handleVector(vf, mesh.positions);
				handleVector(vf, mesh.normals);
				handleVector(vf, mesh.texcoords);
				handleVector(vf, mesh.bones);
				handleVector(vf, mesh.materials);
			}
		};

		void Write(const mesh::Mesh& mesh, const pwn::string& filename)
		{
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, false);
			MeshFile<const mesh::Mesh&>::handle(vf, mesh, version);
		}

		void Read(mesh::Mesh* mesh, const pwn::string& filename)
		{
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, true);
			MeshFile<mesh::Mesh&>::handle(vf, *mesh, version);
		}
	}
}