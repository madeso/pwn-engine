#include <pwn/render/widget.h>

namespace pwn
{
	namespace render
	{
		Widget::Widget(const math::rect& rect)
			: rect(rect)
		{
		}

		Widget::~Widget()
		{
		}
	}
}