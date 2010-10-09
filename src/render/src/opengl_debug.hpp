#ifndef PWN_RENDER_OPENGL_DEBUG_HPP
#define PWN_RENDER_OPENGL_DEBUG_HPP

namespace pwn
{
	namespace render
	{
		/** returns glGetError() and outputs the result if fatal to some debug output
		*/
		GLenum glGetError_WithString();
	}
}

#endif