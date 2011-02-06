#include <pwn/render/dock.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		Dock Dock::Fill()
		{
			Dock fill;
			return fill;
		}

		math::rect Dock::getRect(int width, int height) const
		{
			return math::FromLrud(0, width, 0, -height);
		}

		Dock::Dock()
		{
		}
	}
}
