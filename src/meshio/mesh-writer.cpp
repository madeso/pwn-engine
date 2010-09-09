#include <pwn/meshio/io.h>

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include "vfs.h"
#include <boost/foreach.hpp>
#include <map>

namespace pwn
{
	namespace meshio
	{
		const pwn::uint8 kVersion = 1;

		template<class MeshArg>
		class MeshFile
		{
		private:
			static mesh::Mesh::TriList& GetTriList(mesh::Mesh::TriangleMap& map, pwn::uint32 id)
			{
				return map[id];
			}

			static const mesh::Mesh::TriList& GetTriList(const mesh::Mesh::TriangleMap& map, pwn::uint32 id)
			{
				mesh::Mesh::TriangleMap::const_iterator r = map.find(id);
				if( r == map.end() ) throw "trilist id invalid";
				return r->second;
			}

		public:
			template <typename VersionType>
			static void handle(VirtualFile& vf, MeshArg mesh, VersionType version)
			{
				vf.handle8(version);
				if( version != kVersion ) throw "mesh version mismatch";
				HandleVector(vf, mesh.positions);
				HandleVector(vf, mesh.normals);
				HandleVector(vf, mesh.texcoords);
				HandleVector(vf, mesh.bones);
				std::vector<pwn::uint32> keys = HandleKeys(vf, mesh.triangles);
				BOOST_FOREACH(pwn::uint32 id, keys)
				{
					HandleVector(vf, GetTriList(mesh.triangles, id));
				}
				HandleVector(vf, mesh.materials);
			}
		};

		void Write(const mesh::Mesh& mesh, const pwn::string& filename)
		{
			VirtualFile vf(filename, false);
			MeshFile<const mesh::Mesh&>::handle<const pwn::uint8>(vf, mesh, kVersion);
		}

		void Read(mesh::Mesh* mesh, const pwn::string& filename)
		{
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, true);
			MeshFile<mesh::Mesh&>::handle<pwn::uint8&>(vf, *mesh, version);
		}
	}
}