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
		// a general open gl image
		class Image
		{
		public:
			Image(bool apha, int width, int height, const byte* bitmapData, bool mipmap, int format);
			~Image();
			void bind(int location) const;

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
			
			void bind(int location) const;
			const uint32 sid() const; ///< sort id
			void setImage(Image* img);
		private:
			const bool useGlTexture;
			const core::Id id;
			std::auto_ptr<Image> texture;
		};

		void Load(Texture2* tex, uint32 width, uint32 height, const byte* pixels);
	}
}

#endif