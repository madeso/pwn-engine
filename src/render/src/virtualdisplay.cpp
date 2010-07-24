#include <pwn/render/virtualdisplay.h>
#include <pwn/render/widget.h>
#pragma warning(disable:4512) // warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

#include <SFML/OpenGl.hpp>
#pragma comment(lib, "opengl32.lib")

namespace pwn
{
	namespace render
	{
		VirtualDisplay::VirtualDisplay()
		{
		}

		VirtualDisplay::~VirtualDisplay()
		{
		}

		void VirtualDisplay::widget_add(WidgetPtr widget)
		{
			widgets.push_back(widget);
		}

		void VirtualDisplay::render(int width, int height)
		{
			glViewport(0,0, width, height);
			glClear( GL_COLOR_BUFFER_BIT );

			BOOST_FOREACH(WidgetPtr w, widgets)
			{
				w->render(width, height);
			}
		}
	}
}
