#include <pwn/render/world3widget>
#include <pwn/render/world3>
#include <pwn/math/operations>

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
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(camera.fov, AspectOf(rect), camera.znear, camera.zfar);
			// todo: test for mask occlusion and possible render to a temporary texture

			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			world->render(camera);
			glDisable(GL_DEPTH_TEST);
		}
	}
}