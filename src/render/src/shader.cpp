#include <pwn/render/shader.h>
#include <boost/property_tree/xml_parser.hpp>
#include <pwn/core/str.h>

namespace pwn
{
	namespace render
	{
		bool CompileStatus(int prog)
		{
			//int status;
			//glGetShaderiv(prog, GL_COMPILE_STATUS, &status);
			//return status == Gl.GL_TRUE;
			return true;
		}

		string GetInfoLog(int prog)
		{
			/*int size;
			glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &size);

			int length;
			boost::scoped_array<char> log(new log[size+1]);
			glGetShaderInfoLog(prog, size, &length, log.get());

			return log.get();*/
			return "";
		}

		ShaderSource::ShaderSource(const string& name, const string& source, Type type)
			: mShader( 0 ) // glCreateShader(type)
		{
			//glShaderSource(mShader, 1, source.c_str(), source.length());
			//glCompileShader(mShader);
			if( false == CompileStatus(mShader) )
			{
				throw
					static_cast<string>(core::Str() << "Shader: "<< name << " failed to compile: " << GetInfoLog(mShader));
			}
		}

		int ShaderSource::getShader() const
		{
			return mShader;
		}

		ShaderPtr Shader::LoadFile(const string& path)
		{
			core::Ptree file;
			read_xml(path, file);
			return Create(file.get_child("shader"));
		}

		ShaderPtr Shader::Create(const core::Ptree& source)
		{
			ShaderPtr sh(new Shader());
			sh->vertex = source.get<string>("vertex");
			sh->fragment = source.get<string>("fragment");
			return sh;
		}

		UniformPtr Shader::getUniform(const string& name)
		{
			UniformPtr ret(new Uniform(this, name) );
			return ret;
		}

		void Shader::Bind(ShaderPtr sh)
		{
		}

		void Shader::Unbind(ShaderPtr sh)
		{
		}

		Shader::Shader()
		{
		}
	}
}
