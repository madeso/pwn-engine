#ifndef PWN_RENDER_TEXTURE2
#define PWN_RENDER_TEXTURE2

#include <pwn/number.h>
#include <pwn/core/idpool.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class Texture2
			: boost::noncopyable
		{
		public:
			Texture2(core::IdPool* pool, bool useGlTexture);
			~Texture2();

			const uint32 tid() const; // texture id
			const uint32 sid() const; // sort id
		private:
			const bool useGlTexture;
			uint32 texture;
			core::Id id;
		};

		void Load(Texture2* tex, uint32 width, uint32 height, const byte* pixels);
	}
}

#endif