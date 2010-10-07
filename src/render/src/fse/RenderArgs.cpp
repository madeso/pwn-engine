﻿#include <pwn/render/fse/RenderArgs.h>
#include <pwn/render/world3.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			RenderArgs::RenderArgs(const CompiledCamera& acam, int awidth, int aheight)
				: cam(acam)
				, width(awidth)
				, height(aheight)
			{
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
			
			void RenderArgs::render(WorldPtr world) const
			{
				world->render(0, 0, width, height, cam);
			}
		}
	}
}
