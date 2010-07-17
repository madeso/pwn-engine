#ifndef PWN_ENGINE_VFSTEXTUREPOOL2
#define PWN_ENGINE_VFSTEXTUREPOOL2

#include <pwn/render/texturepool2.h>

namespace pwn
{
	namespace engine
	{
		class VfsTexturePool2
			: public render::TexturePool2
		{
		public:
			VfsTexturePool2();
			~VfsTexturePool2();
		protected:
			boost::shared_ptr<render::Texture2> doLoad(core::IdPool* pool, const string&);
		};
	}
}

#endif