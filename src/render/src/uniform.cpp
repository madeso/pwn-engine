#include <pwn/render/uniform.h>
#include <pwn/core/str.h>
#include <pwn/render/shader.h>
#include <pwn/math/types.h>
#include "opengl_debug.hpp"

namespace pwn
{
	namespace render
	{
		Uniform::Uniform(Shader* sh, const string& name)
			: var(glGetUniformLocation(sh->getProgram(), name.c_str()))
		{
			pwnAssert_NoGLError();
			if( var == -1 ) throw static_cast<string>(core::Str() << name << " is not a recognized uniform");
		}

		void Uniform::bindUniform(int location)
		{
			glUniform1i(var, location);
			pwnAssert_NoGLError();
		}

		void Uniform::bindUniform(float value)
		{
			glUniform1f(var, value);
			pwnAssert_NoGLError();
		}

		void Uniform::bindUniform(const math::vec2& v)
		{
			glUniform2f(var, v.x, v.y);
			pwnAssert_NoGLError();
		}

		int Uniform::getLocation() const
		{
			return var;
		}
	}
}
