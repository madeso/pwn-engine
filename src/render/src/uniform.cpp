#include <pwn/render/uniform.h>
#include <pwn/core/str.h>

namespace pwn
{
	namespace render
	{
		Uniform::Uniform(Shader* sh, const string& name)
			: var(0) // glGetUniformLocation(s->program, name)
		{
			if( var == -1 ) throw static_cast<string>(core::Str() << name << " is not a recognized uniform");
		}

		void Uniform::bindUniform(int location)
		{
			// glUniform1i(var, location);
		}

		void Uniform::bindUniform(float value)
		{
			// glUniform1f(var, value);
		}

		void Uniform::bindUniform(const math::vec2& v)
		{
			// glUniform2f(var, v.x, v.y);
		}

		int Uniform::getLocation() const
		{
			return var;
		}
	}
}
