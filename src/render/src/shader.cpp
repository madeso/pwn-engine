#include <pwn/render/shader.h>
#include <pwn/core/str.h>
#include <pwn/io/config.h>
#include <boost/scoped_array.hpp>
#include <GL/glew.h>
#include "opengl_debug.hpp"

namespace pwn
{
	namespace render
	{
		bool CompileStatus(int prog)
		{
			int status;
			glGetShaderiv(prog, GL_COMPILE_STATUS, &status);
			return status == GL_TRUE;
		}

		string GetInfoLog(int prog)
		{
			int size;
			glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &size);

			int length;
			boost::scoped_array<char> log(new char[size+1]);
			glGetShaderInfoLog(prog, size, &length, log.get());

			return log.get();
		}

		ShaderSource::ShaderSource(const string& name, Type shaderType, const string& source)
			: mShader( glCreateShader(shaderType == Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER) )
		{
			GLint length = source.length();
			const char* sourcep = source.c_str();
			glShaderSource(mShader, 1, &sourcep, &length);
			glCompileShader(mShader);
			if( false == CompileStatus(mShader) )
			{
				throw
					static_cast<string>(core::Str() << "Shader: "<< name << " failed to compile: " << GetInfoLog(mShader));
			}
		}

		ShaderSource::~ShaderSource()
		{
			glDeleteShader(mShader);
		}

		int ShaderSource::getShader() const
		{
			return mShader;
		}

		ShaderPtr Shader::LoadFile(const string& path)
		{
			core::Ptree file;
			io::Read(path, &file);
			return Create(file.get_child("shader"));
		}

		ShaderPtr Shader::Create(const core::Ptree& source)
		{
			ShaderPtr sh(new Shader());
			sh->vertex.reset( new ShaderSource("vertex", ShaderSource::Vertex, source.get<string>("vertex")));
			sh->fragment.reset( new ShaderSource("fragment", ShaderSource::Fragment, source.get<string>("fragment")));

			glAttachShader(sh->getProgram(), sh->vertex->getShader());
			glAttachShader(sh->getProgram(), sh->fragment->getShader());
			glLinkProgram(sh->getProgram());
			return sh;
		}

		UniformPtr Shader::getUniform(const string& name)
		{
			UniformPtr ret(new Uniform(this, name) );
			return ret;
		}

		void Shader::Bind(ShaderPtr sh)
		{
			if( sh )
			{
				glUseProgram(sh->getProgram());
			}
		}

		void Shader::Unbind(ShaderPtr sh)
		{
			glUseProgram(0);
		}

		Shader::Shader()
			: program( glCreateProgram() )
		{
		}

		int Shader::getProgram() const
		{
			return program;
		}

		bool Shader::IsSupported()
		{
			//const bool hasVertex = GLEW_ARB_vertex_shader;
			//const bool hasFragment = GLEW_ARB_fragment_shader;
			//return hasVertex && hasFragment;
			return GLEW_VERSION_2_0;
		}
	}
}
