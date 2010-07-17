#include <pwn/render/world2.h>
#include <pwn/render/widget.h>
#pragma warning(disable:4512) // warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

#include <SFML/OpenGl.hpp>
#pragma comment(lib, "opengl32.lib")

namespace pwn
{
	namespace render
	{
		World2::World2(int width, int height)
			: width(width)
			, height(height)
		{
		}

		World2::~World2()
		{
		}

		void World2::widget_add(WidgetPtr widget)
		{
			widgets.push_back(widget);
		}

		int World2::getWidth() const
		{
			return width;
		}

		int World2::getHeight() const
		{
			return height;
		}

		void World2::render()
		{
			glViewport(0,0, width, height);
			glClear( GL_COLOR_BUFFER_BIT );

			BOOST_FOREACH(WidgetPtr w, widgets)
			{
				w->render();
			}
		}
	}
}
