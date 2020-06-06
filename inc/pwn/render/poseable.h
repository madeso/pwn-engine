#ifndef PWN_RENDER_POSEABLE
#define PWN_RENDER_POSEABLE

#include <boost/noncopyable.hpp>
#include <pwn/math/types.h>
#include <pwn/mesh/mesh.h>

namespace pwn
{
	namespace render
	{
		/** Something that is poseable.
		*/
		class Poseable
			: boost::noncopyable
		{
		public:
			Poseable();
			mesh::CompiledPose pose;
		};
	}
}

#endif
