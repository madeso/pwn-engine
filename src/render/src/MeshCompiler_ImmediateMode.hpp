#ifndef PWN_RENDER_MESHCOMPILER_IM_HPP
#define PWN_RENDER_MESHCOMPILER_IM_HPP
#include "MeshCompiler.hpp"

namespace pwn
{
namespace render
{
    MeshCompiler*
    CreateMeshCompiler_ImmediateMode(const mesh::Mesh& mesh);
}
}

#endif
