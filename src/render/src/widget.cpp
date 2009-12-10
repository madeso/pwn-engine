#include <pwn/render/widget>

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