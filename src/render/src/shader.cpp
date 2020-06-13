#include <pwn/render/shader.h>
#include <pwn/core/str.h>
#include <pwn/io/config.h>
#include <pwn/core/str.h>
#include <boost/scoped_array.hpp>
#include <GL/glew.h>
#include "opengl_debug.hpp"

namespace pwn
{
namespace render
{
    bool
    CompileStatus(int prog)
    {
        int status = GL_FALSE;
        glGetShaderiv(prog, GL_COMPILE_STATUS, &status);
        pwnAssert_NoGLError();
        return status == GL_TRUE;
    }

    string
    GetInfoLog(int prog)
    {
        int size = 0;
        glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &size);
        pwnAssert_NoGLError();

        int length = -1;
        boost::scoped_array<char> log(new char[size + 1]);
        glGetShaderInfoLog(prog, size, &length, log.get());
        pwnAssert_NoGLError();

        if (length == -1)
        {
            return "";
        }

        return log.get();
    }

    ShaderSource::ShaderSource(
            const string& name,
            Type shaderType,
            const string& source)
        : mShader(glCreateShader(
                  shaderType == Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER))
    {
        pwnAssert_NoGLError();
        if (mShader == 0)
        {
            throw "failed to create shader";
        }
        GLint length = source.length();
        const char* sourcep = source.c_str();
        glShaderSource(mShader, 1, &sourcep, &length);
        pwnAssert_NoGLError();
        glCompileShader(mShader);
        pwnAssert_NoGLError();
        if (false == CompileStatus(mShader))
        {
            throw static_cast<string>(
                    core::Str() << "Shader: " << name << " failed to compile: "
                                << GetInfoLog(mShader));
        }
    }

    ShaderSource::~ShaderSource()
    {
        glDeleteShader(mShader);
        pwnAssert_NoGLError();
    }

    int
    ShaderSource::getShader() const
    {
        return mShader;
    }

    ShaderPtr
    Shader::LoadFile(const string& path)
    {
        core::Ptree file;
        io::Read(path, &file);
        return Create(file.get_child("shader"), path);
    }

    ShaderPtr
    Shader::Create(const core::Ptree& source, const string& id)
    {
        ShaderPtr sh(new Shader());
        sh->vertex.reset(new ShaderSource(
                core::Str() << id << "#vertex",
                ShaderSource::Vertex,
                source.get<string>("vertex")));
        sh->fragment.reset(new ShaderSource(
                core::Str() << id << "#fragment",
                ShaderSource::Fragment,
                source.get<string>("fragment")));

        glAttachShader(sh->getProgram(), sh->vertex->getShader());
        pwnAssert_NoGLError();
        glAttachShader(sh->getProgram(), sh->fragment->getShader());
        pwnAssert_NoGLError();
        glLinkProgram(sh->getProgram());
        pwnAssert_NoGLError();
        return sh;
    }

    UniformPtr
    Shader::getUniform(const string& name)
    {
        UniformPtr ret(new Uniform(this, name));
        return ret;
    }

    void
    Shader::Bind(ShaderPtr sh)
    {
        if (sh)
        {
            glUseProgram(sh->getProgram());
            pwnAssert_NoGLError();
        }
    }

    void
    Shader::Unbind(ShaderPtr sh)
    {
        glUseProgram(0);
        pwnAssert_NoGLError();
    }

    Shader::Shader() : program(glCreateProgram())
    {
        pwnAssert_NoGLError();
        if (program == 0)
        {
            throw "failed to create shader program";
        }
    }

    Shader::~Shader()
    {
        glDeleteProgram(program);
        pwnAssert_NoGLError();
    }

    int
    Shader::getProgram() const
    {
        return program;
    }

    bool
    Shader::IsSupported()
    {
        //const bool hasVertex = GLEW_ARB_vertex_shader;
        //const bool hasFragment = GLEW_ARB_fragment_shader;
        //return hasVertex && hasFragment;
        return GLEW_VERSION_2_0 == GL_TRUE;
    }
}
}
