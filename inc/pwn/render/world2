#ifndef PWN_RENDER_WORLD2
#define PWN_RENDER_WORLD2

#include <boost/shared_ptr.hpp>
#include <vector>

namespace pwn
{
	namespace render
	{
		class Widget;

		class World2
		{
		public:
			World2(int width, int height);
			~World2();

			typedef boost::shared_ptr<Widget> WidgetPtr;

			void widget_add(WidgetPtr widget);

			int getWidth() const;
			int getHeight() const;

			void render();
		private:
			int width;
			int height;
			std::vector<WidgetPtr> widgets;
		};
	}
}

#endif