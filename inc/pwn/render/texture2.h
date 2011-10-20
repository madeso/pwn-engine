#ifndef PWN_RENDER_TEXTURE2
#define PWN_RENDER_TEXTURE2

#include <pwn/number.h>
#include <pwn/core/idpool.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

namespace pwn
{
	namespace render
	{
		class Engine;

		real GetMaxAnistropy();

		// a general open gl image
		class Image
			: boost::noncopyable
		{
		public:
			Image(bool alpha, int width, int height, const byte* bitmapData, bool mipmap, int format, real anistropy, bool compress);
			~Image();
			void bind(int position) const;

			unsigned int getId() const;

			static bool IsSupported();
		private:
			unsigned int text;
		};

		/** A loaded texture.
		*/
		class Texture2
			: boost::noncopyable
		{
		public:
			Texture2(core::IdPool* pool, bool useGlTexture);
			~Texture2();

			void bind(int position) const;
			const uint32 sid() const; ///< sort id
			void setImage(Image* img);
		private:
			const bool useGlTexture;
			const core::Id id;
			std::auto_ptr<Image> texture;
		};

		void Load(Texture2* tex, uint32 width, uint32 height, const byte* pixels, const Engine& engine);
	}
}

#endif
