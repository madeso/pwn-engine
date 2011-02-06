#include <pwn/render/shaderpool.h>
#include <pwn/render/shader.h>

namespace pwn
{
	namespace render
	{
		ShaderPtr ShaderPool::getFromFile(const string& file)
		{
			ShaderPtr loaded = get(file);
			if( loaded ) return loaded;
			else return add(file, Shader::LoadFile(file));
		}

		ShaderPtr ShaderPool::getFromSource(const core::Ptree& source, const string& id)
		{
			ShaderPtr loaded = get(id);
			if( loaded ) return loaded;
			else return add(id, Shader::Create(source, id));
		}

		ShaderPtr ShaderPool::get(const string& id)
		{
			Map::iterator res = shaders.find(id);
			if( res == shaders.end() ) return ShaderPtr();
			else return res->second.lock();
		}

		ShaderPtr ShaderPool::add(const string& id, ShaderPtr sh)
		{
			shaders.insert( Map::value_type(id, sh) );
			return sh;
		}
	}
}
