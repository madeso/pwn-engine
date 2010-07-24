#ifndef PWN_RENDER_WIDGET
#define PWN_RENDER_WIDGET

#include <pwn/render/dock.h>

namespace pwn
{
	namespace render
	{
		class Widget
		{
		public:
			Widget(const Dock& dock);
			virtual ~Widget();

			virtual void render(int width, int height) = 0;
		protected:
			Dock dock;
		};
	}
}

#endif