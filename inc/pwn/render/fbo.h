#ifndef PWN_RENDER_FBO
#define PWN_RENDER_FBO

#include <boost/utility.hpp>

namespace pwn
{
	namespace render
	{
		// update this class...
		class Fbo
			: boost::noncopyable
		{
		public:
			Fbo(int w, int h, bool arg);
			void bindTexture(int location);
			int getWidth() const;
			int getHeight() const;
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
