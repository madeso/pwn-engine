#include <pwn/render/utils.h>
#include <pwn/string.h>
#include <pwn/render/lines.h>
#include <pwn/math/operations.h>

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

		void SetupGrid(World3::Ptr world, int halfCount, real swidth, real fwidth, int mod, real size)
		{
			Lines::Ptr small = Lines::Create(swidth);
			Lines::Ptr fat = Lines::Create(fwidth);

			real dist = 20;

			for(int i=-halfCount; i<halfCount; ++i)
			{
				const real d = dist * i;
				Lines::Ptr lines = (i % mod == mod-1)?fat:small;
				lines->add(math::Left()*-size + math::In()*d, math::Left()*size + math::In()*d);
				lines->add(math::In()*-size + math::Left()*d, math::In()*size + math::Left()*d);
			}

			world->lines_add(small);
			world->lines_add(fat);
		}
	}

	string Nullstring(const string& str, const string& def)
	{
		if( str.empty()==false ) return str;
		else return def;
	}
}
