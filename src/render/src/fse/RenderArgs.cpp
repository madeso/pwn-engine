#include <pwn/render/fse/RenderArgs.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			RenderArgs::RenderArgs(WorldPtr aworld, const CompiledCamera& acam, int awidth, int aheight)
				: world(aworld)
				, cam(acam)
				, width(awidth)
				, height(aheight)
			{
			}
			
			WorldPtr RenderArgs::getWorld()
			{
				return world;
			}
			
			const CompiledCamera& RenderArgs::getCamera() const
			{
				return cam;
			}
			
			int RenderArgs::getWidth()
			{
				return width;
			}
			
			int RenderArgs::getHeight()
			{
				return height;
			}
			
			void RenderArgs::render()
			{
				world->render(width, height, cam);
			}
		}
	}
}
