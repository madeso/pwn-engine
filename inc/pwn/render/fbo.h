#ifndef PWN_RENDER_FBO
#define PWN_RENDER_FBO

#include <boost/utility.hpp>
#include <pwn/render/texture2.h>
#include <memory>

namespace pwn
{
	namespace render
	{
		class RenderBuffer
			: boost::noncopyable
		{
		public:
			RenderBuffer(int internalFormat, int width, int height);
			~RenderBuffer();
			void bind();
			unsigned int getBuffer() const;
		private:
			unsigned int buffer;
		};

		class Fbo
			: boost::noncopyable
		{
		public:
			Fbo(int w, int h, bool mipmap);
			void bindTexture(int position);
			int getWidth() const;
			int getHeight() const;
			int getId() const;

			static bool IsSupported();
		protected:
			void attach(RenderBuffer* buffer, int point);
		private:
			unsigned int fbo;
			int width;
			int height;
			std::auto_ptr<RenderBuffer> depth;
			std::auto_ptr<Image> texture;
		};

		// raii
		class TextureUpdator
			: boost::noncopyable
		{
		public:
			explicit TextureUpdator(Fbo* fbo);
			~TextureUpdator();
		};
	}
}

#endif
