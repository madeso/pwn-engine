#include <pwn/render/shaderpool.h>
#include <pwn/render/shader.h>

namespace pwn
{
	namespace render
	{
		ShaderPtr ShaderPool::shaderFromFile(const string& file)
		{
			ShaderPtr loaded = get(file);
			if( loaded ) return loaded;
			else return Shader::LoadFile(file);
		}

		ShaderPtr ShaderPool::shaderFromSource(const core::Ptree& source, const string& id)
		{
			ShaderPtr loaded = get(id);
			if( loaded ) return loaded;
			else return Shader::Create(source, id);
		}

		ShaderPtr ShaderPool::get(const string& id)
		{
			Map::iterator res = shaders.find(id);
			if( res == shaders.end() ) return ShaderPtr();
			else return res->second.lock();
		}
	}
}
