#ifndef PWN_RENDER_UNIFORM
#define PWN_RENDER_UNIFORM

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <pwn/string.h>

namespace pwn
{
	namespace math
	{
		struct vec2;
	}

	namespace render
	{
		class Shader;

		class Uniform
			: boost::noncopyable
		{
		public:
			Uniform(Shader* sh, const string& name);
			void bindUniform(int location);
			void bindUniform(float value);
			void bindUniform(const math::vec2& v);
			int getLocation() const;
		private:
			int var;
			string name;
		};

		typedef boost::shared_ptr<render::Uniform> UniformPtr;
	}
}

#endif
