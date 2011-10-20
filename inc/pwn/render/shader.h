#ifndef PWN_RENDER_SHADER
#define PWN_RENDER_SHADER

#include <boost/utility.hpp>
#include <pwn/string.h>
#include <pwn/render/uniform.h>
#include <pwn/core/config.h>

namespace pwn
{
	namespace render
	{
		class Shader;
		class ShaderSource;
		typedef boost::shared_ptr<render::Shader> ShaderPtr;
		typedef boost::shared_ptr<render::ShaderSource> ShaderSourcePtr;

		/** The compiled source to a shader.
		*/
		class ShaderSource
			: boost::noncopyable
		{
		public:
			enum Type
			{
			   Vertex
			   , Fragment
			};

			ShaderSource(const string& name, Type type, const string& source);
			~ShaderSource();
			int getShader() const;
		private:
			int mShader;
		};

		/// The shader
		class Shader
			: boost::noncopyable
		{
		public:
			static ShaderPtr LoadFile(const string& file);
			static ShaderPtr Create(const core::Ptree& source, const string& id);

			UniformPtr getUniform(const string& name);

			/** Binds a shader.
			Doesnt do anything if the shader is null
			*/
			static void Bind(ShaderPtr sh);

			/** Unbinds a shader.
			Doesnt do anything if the shader is null
			*/
			static void Unbind(ShaderPtr sh);

			int getProgram() const;

			static bool IsSupported();

			~Shader();
		private:
			int program;
			ShaderSourcePtr vertex;
			ShaderSourcePtr fragment;
			Shader();
		};
	}
}

#endif
