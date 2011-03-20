#include <pwn/io/io.h>

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include "vfs.hpp"
#include <boost/foreach.hpp>
#include <map>
#include <pwn/assert.h>

namespace pwn
{
	namespace io
	{
		const pwn::uint8 kVersion = 1;

		namespace
		{
			template<class Filer>
			struct MeshUtil {};

			template<>
			struct MeshUtil<FileReader> {
				static mesh::Mesh::TriList& GetTriList(mesh::Mesh::TriangleMap& map, pwn::uint32 id)
				{
					return map[id];
				}
			};

			template<>
			struct MeshUtil<FileWriter> {
				static const mesh::Mesh::TriList& GetTriList(const mesh::Mesh::TriangleMap& map, pwn::uint32 id)
				{
					mesh::Mesh::TriangleMap::const_iterator r = map.find(id);
					if( r == map.end() ) throw "trilist id invalid";
					return r->second;
				}
			};
		}

		template<class MeshArg, typename VersionType>
		class MeshFile
		{
		public:
			template <class Filer>
			static void handle(Filer& vf, MeshArg mesh, VersionType version)
			{
				vf.handle8(version);
				if( version != kVersion ) throw "mesh version mismatch";
				vf.handle32(mesh.vertexes.count);
				vf.handleArray(mesh.vertexes.locations, mesh.vertexes.count*3);
				vf.handleArray(mesh.vertexes.normals, mesh.vertexes.count*3);
				vf.handleArray(mesh.vertexes.textcoords, mesh.vertexes.count*2);
				vf.handleArray(mesh.vertexes.boneindexes, mesh.vertexes.count*4);
				vf.handleVector(mesh.bones);
				std::vector<pwn::uint32> keys = vf.handleKeys(mesh.triangles);
				BOOST_FOREACH(pwn::uint32 id, keys)
				{
					vf.handleVector(MeshUtil<Filer>::GetTriList(mesh.triangles, id));
				}
				vf.handleVector(mesh.materials);
			}
		};

		void Write(const mesh::Mesh& mesh, const pwn::string& filename)
		{
			Assert(mesh.validate(true) == 0);
			VirtualFile vf(filename, false);
			FileWriter w;
			w.file = &vf;
			MeshFile<const mesh::Mesh&,const pwn::uint8>::handle(w, mesh, kVersion);
		}

		void Read(mesh::Mesh* mesh, const pwn::string& filename)
		{
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, true);
			FileReader r;
			r.file = &vf;
			MeshFile<mesh::Mesh&,pwn::uint8&>::handle(r, *mesh, version);
			Assert(mesh->validate(true) == 0);
		}
	}
}
