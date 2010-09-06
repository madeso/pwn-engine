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
			template<class T>
			static void handleVector(VirtualFile& vf, const std::vector<T>& vector)
			{
				pwn::uint32 size = vector.size();
				vf.write32(size);
				if( size > 0 )
				{
					vf.write(&vector[0], size * sizeof(T));
				}
			}

			template<class T>
			static void handleVector(VirtualFile& vf, std::vector<T>& vector)
			{
				pwn::uint32 size = vf.read32();
				vector.resize(size);
				if( size > 0 )
				{
					vf.read(&vector[0], size * sizeof(T));
				}
			}

			template<class Value>
			static std::vector<pwn::uint32> handleKeys(VirtualFile& vf, const std::map<pwn::uint32, Value>& map)
			{
				pwn::uint32 size = map.size();
				vf.write32(size);
				std::vector<pwn::uint32> ret;
				for(std::map<pwn::uint32, Value>::const_iterator i = map.begin(); i != map.end(); ++i)
				{
					const pwn::uint32 v = i->first;
					vf.write32(v);
					ret.push_back(v);
				}
				return ret;
			}

			template<class V>
			static std::vector<pwn::uint32> handleKeys(VirtualFile& vf, std::map<pwn::uint32, V>&)
			{
				pwn::uint32 size = vf.read32();
				std::vector<pwn::uint32> ret;
				ret.resize(size);
				for(pwn::uint32 i=0; i<size; ++i)
				{
					ret[i] = vf.read32();
				}
				return ret;
			}

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
				handleVector(vf, mesh.positions);
				handleVector(vf, mesh.normals);
				handleVector(vf, mesh.texcoords);
				handleVector(vf, mesh.bones);
				std::vector<pwn::uint32> keys = handleKeys(vf, mesh.triangles);
				BOOST_FOREACH(pwn::uint32 id, keys)
				{
					handleVector(vf, GetTriList(mesh.triangles, id));
				}
				handleVector(vf, mesh.materials);
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