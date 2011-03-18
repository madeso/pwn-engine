#ifndef PWN_RENDER_MESHCOMPILER_HPP
#define PWN_RENDER_MESHCOMPILER_HPP

#include <pwn/mesh/mesh.h>
#include <pwn/render/compiledmesh.h>

namespace pwn
{
	namespace render
	{
		class MeshCompiler
		{
		public:
			MeshCompiler();
			virtual ~MeshCompiler();

			virtual CompiledMesh* compile(const mesh::Mesh::TriList& tridata) = 0;
		};
	}
}

#endif
