#ifndef PWN_RENDER_UNIFORM
#define PWN_RENDER_UNIFORM

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <pwn/string.h>
#include <pwn/math/types.h>

namespace pwn
{
	namespace render
	{
		class Shader;

		/** A shader uniform.
		*/
		class Uniform
			: boost::noncopyable
		{
		public:
			Uniform(Shader* sh, const string& name);
			void bindUniform(int position);
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
