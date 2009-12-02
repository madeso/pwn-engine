#include <pwn/engine/vfstexturepool2>
#include <pwn/render/Texture2>

#include "vfs_util.hpp"

#include<soil/soil.h>

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

		struct Soil
		{
			explicit Soil(ubyte* pixels)
				: pixels(pixels)
			{
				if( pixels == 0)
				{
					pwn::string error = SOIL_last_result();
					throw "Failed to load b/c " + error;
				}
			}

			~Soil()
			{
				SOIL_free_image_data(pixels);
			}

			ubyte* pixels;
		};

		boost::shared_ptr<render::Texture2> VfsTexturePool2::doLoad(core::IdPool* pool, const string& filename)
		{
			// sf::Image img;
			boost::scoped_array<byte> memory;
			// todo: replace sfml loading with a better version
			const std::size_t size = File(PHYSFS_openRead(filename.c_str())).loadToMemory(&memory);
			boost::shared_ptr<render::Texture2> tex( new render::Texture2(pool) );
			int width = -1;
			int height = -1;
			int channels = -1;
			Soil soil( SOIL_load_image_from_memory(reinterpret_cast<ubyte*>(memory.get()), size, &width, &height, &channels, SOIL_LOAD_RGBA) );
			render::Load(tex.get(), width, height, reinterpret_cast<byte*>(soil.pixels));
			return tex;
		}
	}
}