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
		typedef boost::shared_ptr<render::Shader> ShaderPtr;

		/** The compiled source to a shader.
		*/
		class ShaderSource
		{
		public:
			enum Type
			{
				Vertex // GL_VERTEX_SHADER
				,Fragment // GL_FRAGMENT_SHADER
			};

			ShaderSource(const string& name, const string& source, Type type);
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
			static ShaderPtr Create(const core::Ptree& source);

			UniformPtr getUniform(const string& name);

			/** Binds a shader.
			Doesnt do anything if the shader is null
			*/
			static void Bind(ShaderPtr sh);

			/** Unbinds a shader.
			Doesnt do anything if the shader is null
			*/
			static void Unbind(ShaderPtr sh);

		private:
			string vertex;
			string fragment;
			Shader();
		};
	}
}

#endif
