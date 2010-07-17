#include <pwn/render/world3widget.h>
#include <pwn/render/world3.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

#include <SFML/OpenGl.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


namespace pwn
{
	namespace render
	{
		World3Widget::World3Widget(const math::rect& rect, boost::shared_ptr<World3> world)
			: Widget(rect)
			, world(world)
			, camera( math::Origo3(), math::qIdentity(), 45.0f, 0.5f, 1000.0f)
		{
		}

		World3Widget::~World3Widget()
		{
		}

		void World3Widget::setCamera(const Camera& camera)
		{
			this->camera = camera;
		}

		void World3Widget::render()
		{
			const int x = static_cast<GLsizei>(LeftOf(rect));
			const int y = -static_cast<GLsizei>(TopOf(rect));
			const int w = static_cast<GLsizei>(WidthOf(rect));
			const int h = static_cast<GLsizei>(HeightOf(rect));
			glViewport(x, y, w, h);
			Assert( glGetError() == GL_NO_ERROR);
			glMatrixMode(GL_PROJECTION);
			Assert( glGetError() == GL_NO_ERROR);
			glLoadIdentity();
			Assert( glGetError() == GL_NO_ERROR);
			gluPerspective(camera.fov, AspectOf(rect), camera.znear, camera.zfar);
			Assert( glGetError() == GL_NO_ERROR);
			// todo: test for mask occlusion and possible render to a temporary texture

			glClear(GL_DEPTH_BUFFER_BIT);
			Assert( glGetError() == GL_NO_ERROR);
			glEnable(GL_DEPTH_TEST);
			Assert( glGetError() == GL_NO_ERROR);
			glEnable(GL_CULL_FACE);
			Assert( glGetError() == GL_NO_ERROR);
			world->render(camera);
			Assert( glGetError() == GL_NO_ERROR);
			glDisable(GL_DEPTH_TEST);
			Assert( glGetError() == GL_NO_ERROR);
		}
	}
}