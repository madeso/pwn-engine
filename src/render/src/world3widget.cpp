#include <pwn/render/world3widget.h>
#include <pwn/render/world3.h>
#include <pwn/math/operations.h>
#include <pwn/render/renderargs.h>
#include <pwn/render/compiledcamera.h>
#include <pwn/render/fse/pipeline.h>

#include "opengl_debug.hpp"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


namespace pwn
{
	namespace render
	{
		World3Widget::World3Widget(const Dock& dock, boost::shared_ptr<World3> world)
			: Widget(dock)
			, world(world)
			, camera( math::Origo3(), math::qIdentity(), 45.0f, 0.5f, 1000.0f)
		{
		}

		World3Widget::~World3Widget()
		{
		}

		void World3Widget::updateCamera(const Camera& camera)
		{
			this->camera = camera;
		}

		void World3Widget::render(int width, int height)
		{
			const math::rect rect = dock.getRect(width, height);
			const int x = static_cast<GLsizei>(LeftOf(rect));
			const int y = -static_cast<GLsizei>(TopOf(rect));
			const int w = static_cast<GLsizei>(WidthOf(rect));
			const int h = static_cast<GLsizei>(HeightOf(rect));
			glMatrixMode(GL_PROJECTION);
			pwnAssert_NoGLError();
			glLoadIdentity();
			pwnAssert_NoGLError();
			gluPerspective(camera.fov, AspectOf(rect), camera.znear, camera.zfar);
			pwnAssert_NoGLError();
			// todo: test for mask occlusion and possible render to a temporary texture

			glClear(GL_DEPTH_BUFFER_BIT);
			pwnAssert_NoGLError();
			glEnable(GL_DEPTH_TEST);
			pwnAssert_NoGLError();
			glEnable(GL_CULL_FACE);
			pwnAssert_NoGLError();
			RenderArgs ra(camera, x, y, w, h);
			//world->render(ra);
			camera.pipeline->render(world.get(), ra);
			pwnAssert_NoGLError();
			glDisable(GL_DEPTH_TEST);
			pwnAssert_NoGLError();
		}
	}
}