#ifndef PWN_RENDER_COMPILEDMESH
#define PWN_RENDER_COMPILEDMESH

#include <boost/noncopyable.hpp>

namespace pwn
{
	namespace mesh
	{
		class CompiledPose;
	}

	namespace render
	{
		/** Optimized version of a mesh used for fast rendering.
		 */
		class CompiledMesh
			: boost::noncopyable
		{
		public:
			CompiledMesh();
			virtual ~CompiledMesh();

			virtual void render(const mesh::CompiledPose& pose) = 0;
		};
	}
}

#endif
