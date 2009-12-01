#include <pwn/engine/vfstexturepool2>
#include <pwn/render/Texture2>

#include "vfs_util.hpp"

#include <SFML/Graphics/Image.hpp>

#ifdef _DEBUG
#define LIBRARY_SUFFIX "-d"
#else
#define LIBRARY_SUFFIX ""
#endif

#pragma comment(lib, "sfml-graphics" LIBRARY_SUFFIX ".lib" )

namespace pwn
{
	namespace engine
	{
		VfsTexturePool2::VfsTexturePool2()
		{
		}

		VfsTexturePool2::~VfsTexturePool2()
		{
		}

		boost::shared_ptr<render::Texture2> VfsTexturePool2::doLoad(core::IdPool* pool, const string& filename)
		{
			sf::Image img;
			boost::scoped_array<byte> memory;
			// todo: replace sfml loading with a better version
			const std::size_t size = File(PHYSFS_openRead(filename.c_str())).loadToMemory(&memory);
			boost::shared_ptr<render::Texture2> tex( new render::Texture2(pool) );
			if( img.LoadFromMemory(memory.get(), size) == false )
			{
				throw "failed to load image";
			}
			render::Load(tex.get(), img.GetWidth(), img.GetHeight(), reinterpret_cast<const byte*>(img.GetPixelsPtr()));
			return tex;
		}
	}
}