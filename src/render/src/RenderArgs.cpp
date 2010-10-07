#include <pwn/render/RenderArgs.h>
#include <pwn/render/world3.h>

namespace pwn
{
	namespace render
	{
		RenderArgs::RenderArgs(const Camera& acam, int ax, int ay, int awidth, int aheight)
			: compiled(acam)
			, camera(acam)
			, x(ax)
			, y(ay)
			, width(awidth)
			, height(aheight)
		{
		}
		
		void RenderArgs::render(World3* world) const
		{
			world->render(*this);
		}
	}
}
