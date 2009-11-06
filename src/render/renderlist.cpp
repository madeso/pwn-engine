#include <pwn/render/renderlist>
#include <pwn/math/operations>

#include <SFML/OpenGl.hpp>

namespace pwn
{
	namespace render
	{
		RenderList::RenderList()
		{
		}
		RenderList::~RenderList()
		{
		}

		// for resize(0) support
		RenderList::Command::Command()
			: location(math::Origo3())
			, rotation(math::qIdentity())
		{
		}

		RenderList::Command::Command(PartPtr part, const math::point3& location, const math::quat& rotation)
			: part(part)
			, location(location)
			, rotation(rotation)
		{
		}

		void RenderList::begin()
		{
			commands.resize(0);
			int iLeft = 0;
			int iRight = 1;
			int iBottom = 0;
			int iTop = 1;
			glLoadIdentity();
			glOrtho(iLeft, iRight, iBottom, iTop, -1, 1);
			glBegin(GL_QUADS);
				glVertex2d(iRight, iTop);
				glVertex2d(iLeft, iTop);
				glVertex2d(iLeft, iBottom);
				glVertex2d(iRight, iBottom);
			glEnd();
			//glClear( GL_COLOR_BUFFER_BIT );
		}

		void RenderList::add(PartPtr part, math::point3 location, math::quat rotation)
		{
			commands.push_back( Command(part, location, rotation) );
		}

		void RenderList::end()
		{
			// todo: sort & send to ogl
		}
	}
}