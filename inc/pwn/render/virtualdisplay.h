#ifndef PWN_RENDER_WORLD2
#define PWN_RENDER_WORLD2

#include <boost/shared_ptr.hpp>
#include <vector>

namespace pwn
{
	namespace render
	{
		class Widget;

		/** A virtual screen, display or window, will probably be displayed in a real window.
		*/
		class VirtualDisplay
		{
		public:
			VirtualDisplay();
			~VirtualDisplay();

			typedef boost::shared_ptr<Widget> WidgetPtr;

			void widget_add(WidgetPtr widget);

			void render(int width, int height);
		private:
			std::vector<WidgetPtr> widgets;
		};
	}
}

#endif