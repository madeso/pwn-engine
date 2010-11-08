#ifndef PWN_RENDER_SHADERPOOL
#define PWN_RENDER_SHADERPOOL

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <pwn/core/config.h>
#include <map>

namespace pwn
{
	namespace render
	{
		class Shader;
		typedef boost::shared_ptr<render::Shader> ShaderPtr;
		
		/// Pool for shaders
		class ShaderPool
		{
		public:
			ShaderPtr shaderFromFile(const string& file);
			ShaderPtr shaderFromSource(const core::Ptree& source, const string& id);

		private:
			ShaderPtr get(const string& id); ///< return a loaded shader or a nullpointer if not loaded, or it went out of scope
			typedef std::map<pwn::string, boost::weak_ptr<Shader> > Map;
			Map shaders;
		};
	}
}

#endif
