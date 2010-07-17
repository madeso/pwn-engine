#ifndef PWN_RENDER_WIDGET
#define PWN_RENDER_WIDGET

#include <pwn/math/types.h>

namespace pwn
{
	namespace render
	{
		class Widget
		{
		public:
			Widget(const math::rect& rect);
			virtual ~Widget();

			virtual void render() = 0;

			math::rect rect;
		};
	}
}

#endif