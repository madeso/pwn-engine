#include <pwn/engine/vfstexturepool2.h>
#include <pwn/render/texture2.h>

#include "vfs_util.hpp"

#include <soil/SOIL.h>

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

    void
    VfsTexturePool2::setEngine(render::Engine* e)
    {
        engine = e;
    }

    struct Soil
    {
        explicit Soil(ubyte* pixels) : pixels(pixels)
        {
            if (pixels == 0)
            {
                const pwn::string error = SOIL_last_result();
                throw "Failed to load b/c " + error;
            }
        }

        ~Soil()
        {
            SOIL_free_image_data(pixels);
        }

        ubyte* pixels;
    };

    boost::shared_ptr<render::Texture2>
    VfsTexturePool2::doLoad(core::IdPool* pool, const string& filename)
    {
        boost::scoped_array<byte> memory;
        const std::size_t size =
                File(PHYSFS_openRead(filename.c_str())).loadToMemory(&memory);
        boost::shared_ptr<render::Texture2> tex(
                new render::Texture2(pool, true));
        int width = -1;
        int height = -1;
        int channels = -1;
        Soil soil(SOIL_load_image_from_memory(
                reinterpret_cast<ubyte*>(memory.get()),
                size,
                &width,
                &height,
                &channels,
                SOIL_LOAD_RGBA));
        render::Load(
                tex.get(),
                width,
                height,
                reinterpret_cast<byte*>(soil.pixels),
                *engine);
        return tex;
    }
}
}
