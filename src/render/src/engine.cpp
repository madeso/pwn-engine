#include <pwn/render/engine.h>

#include <pwn/string.h>
#include <pwn/render/shader.h>
#include <pwn/render/texture2.h>
#include <pwn/render/fbo.h>
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
            pwn::string error =
                    reinterpret_cast<const char*>(glewGetErrorString(err));
            throw error;
        }

        if (false == Shader::IsSupported())
        {
            throw "Shaders unsupported";
        }

        if (false == Fbo::IsSupported())
        {
            throw "Fbos not supported";
        }

        if (false == Image::IsSupported())
        {
            throw "Images not supported";
        }
    }

    Engine::~Engine()
    {
    }

    real
    Engine::getAnistropy() const
    {
        return GetMaxAnistropy();
    }
}
}
