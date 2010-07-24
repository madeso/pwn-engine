#include <pwn/render/widget.h>

namespace pwn
{
	namespace render
	{
		Widget::Widget(const Dock& adock)
			: dock(adock)
		{
		}

		Widget::~Widget()
		{
		}
	}
}