#ifndef PWN_RENDER_WORLD2
#define PWN_RENDER_WORLD2

#include <boost/shared_ptr.hpp>
#include <vector>

namespace pwn
{
	namespace render
	{
		class Widget;

		class VirtualDisplay
		{
		public:
			VirtualDisplay(int width, int height);
			~VirtualDisplay();

			typedef boost::shared_ptr<Widget> WidgetPtr;

			void widget_add(WidgetPtr widget);

			int getWidth() const;
			int getHeight() const;

			void render(int width, int height);
		private:
			int width;
			int height;
			std::vector<WidgetPtr> widgets;
		};
	}
}

#endif