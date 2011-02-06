#include <pwn/render/RenderArgs.h>
#include <pwn/render/world3.h>

#include "opengl_debug.hpp"

namespace pwn
{
	namespace render
	{
		RenderArgs::RenderArgs(const Camera& acam, int ax, int ay, int awidth, int aheight, real aaspect)
			: compiled(acam)
			, camera(acam)
			, x(ax)
			, y(ay)
			, width(awidth)
			, height(aheight)
			, aspect(aaspect)
		{
		}

		RenderArgs::RenderArgs(const RenderArgs& ra, int awidth, int aheight)
			: compiled(ra.camera)
			, camera(ra.camera)
			, x(0)
			, y(0)
			, width(awidth)
			, height(aheight)
			, aspect(ra.aspect)
		{
		}

		void RenderArgs::render(World3* world) const
		{
			glMatrixMode(GL_PROJECTION); pwnAssert_NoGLError();
			glLoadIdentity(); pwnAssert_NoGLError();
			gluPerspective(camera.fov, aspect, camera.znear, camera.zfar); pwnAssert_NoGLError();
			glClear(GL_DEPTH_BUFFER_BIT); pwnAssert_NoGLError();

			glEnable(GL_DEPTH_TEST); pwnAssert_NoGLError();
			glEnable(GL_CULL_FACE); pwnAssert_NoGLError();
			world->render(*this);
			glDisable(GL_DEPTH_TEST); pwnAssert_NoGLError();
		}
	}
}
