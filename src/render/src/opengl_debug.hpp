#ifndef PWN_RENDER_OPENGL_DEBUG_HPP
#define PWN_RENDER_OPENGL_DEBUG_HPP

#include <pwn/assert.h>
#include <GL/glew.h>

namespace pwn
{
	namespace render
	{
		/** returns glGetError() and outputs the result if fatal to some debug output
		*/
		GLenum glGetError_WithString();
	}
}

#define pwnAssert_NoGLError() Assert( ::pwn::render::glGetError_WithString() == GL_NO_ERROR)

#endif