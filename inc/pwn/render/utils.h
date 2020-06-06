#ifndef PWN_RENDER_UTILS
#define PWN_RENDER_UTILS

#include <boost/utility.hpp>
#include <pwn/math/types.h>
#include <pwn/render/world3.h>

namespace pwn
{
	namespace render
	{
		void RenderFullscreenQuad(int unknown, int width, int height);
		void SetupGrid(World3::Ptr world, int halfCount, real swidth, real fwidth, int mod, real size);
	}
}

#endif
