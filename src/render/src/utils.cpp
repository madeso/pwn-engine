#include <pwn/render/utils.h>
#include <pwn/string.h>

#include "opengl_debug.hpp"

namespace pwn
{
	namespace render
	{
		void SetView2d(int width, int height)
		{
			glMatrixMode(GL_PROJECTION); pwnAssert_NoGLError();
			glLoadIdentity(); pwnAssert_NoGLError();
			glOrtho(0, width, height, 0, 0, 1); pwnAssert_NoGLError();
			glMatrixMode(GL_MODELVIEW); pwnAssert_NoGLError();
			glLoadIdentity(); pwnAssert_NoGLError();
		}

		void RenderFullscreenQuad(int unknown, int width, int height)
		{
			SetView2d(width, height);

			const GLfloat w = static_cast<GLfloat>(width);
			const GLfloat h = static_cast<GLfloat>(height);

			glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex2f(0, 0); // top left
			glTexCoord2f(0, 0); glVertex2f(0, h); // bottom left
			glTexCoord2f(1, 0); glVertex2f(w, h); // bottom right
			glTexCoord2f(1, 1); glVertex2f(w, 0); // top right
			glEnd(); pwnAssert_NoGLError();
		}
	}

	string Nullstring(const string& str, const string& def)
	{
		if( str.empty()==false ) return str;
		else return def;
	}
}
