#include <pwn/render/texturepool2>

#include <pwn/render/texture2>

namespace pwn
{
	namespace render
	{
		TexturePool2::TexturePool2()
		{
		}

		TexturePool2::~TexturePool2()
		{
		}

		boost::shared_ptr<Texture2> TexturePool2::get(const string& path)
		{
			Map::iterator it = map.find(path);
			if( it != map.end() )
			{
				boost::shared_ptr<Texture2> pt = it->second.lock();
				if( pt ) return pt;
			}
			boost::shared_ptr<Texture2> pt = doLoad(&ids, path);
			map[path] = pt;
			return pt;
		}
	}
}