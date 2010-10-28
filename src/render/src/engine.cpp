#include <pwn/render/engine.h>

#include <pwn/string.h>
#include <pwn/render/shader.h>
#include <GL/glew.h>

namespace pwn
{
	namespace render
	{
		Engine::Engine()
		{
			const GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				pwn::string error = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw error;
			}

			if( false == Shader::IsSupported() )
			{
				throw "Shaders unsupported";
			}
		}

		Engine::~Engine()
		{
		}
	}
}

