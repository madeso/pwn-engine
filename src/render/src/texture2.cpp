#include <pwn/render/texture2.h>
#include <pwn/render/engine.h>

#include "opengl_debug.hpp"
#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")

namespace pwn
{
	namespace render
	{
		real GetMaxAnistropy()
		{
			GLfloat anisotropy = 1;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy); pwnAssert_NoGLError();
			return anisotropy;
		}

		Image::Image(bool alpha, int width, int height, const byte* bitmapData, bool mipmap, int format, real anistropy, bool compress)
			: text(0)
		{
			glGenTextures(1, &text); pwnAssert_NoGLError();
			bind(0);
			const bool supportCompress = GLEW_ARB_texture_compression || GLEW_VERSION_1_3;
			const bool doCompress = compress && supportCompress;
			const GLint internalFormat_nc = alpha ? GL_RGBA8 : GL_RGB8;
			const GLint internalFormat_c = alpha ? GL_COMPRESSED_RGBA : GL_COMPRESSED_RGB;
			const GLint internalFormat = doCompress ? internalFormat_c : internalFormat_nc;

			//glEnable(GL_TEXTURE_2D); // might help on certain gfx cards
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); pwnAssert_NoGLError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); pwnAssert_NoGLError();
			
			const GLint minFilter = mipmap? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); pwnAssert_NoGLError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); pwnAssert_NoGLError();

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropy); pwnAssert_NoGLError();

			const int gmipmap = mipmap ? GL_TRUE : GL_FALSE;
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, gmipmap); pwnAssert_NoGLError();
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, bitmapData); pwnAssert_NoGLError();

			if( doCompress )
			{
				GLint result = GL_FALSE;
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &result);  pwnAssert_NoGLError();
				if( result != GL_TRUE )
				{
					throw "failed to compress image";
				}
			}
		}

		Image::~Image()
		{
			glDeleteTextures(1, &text); pwnAssert_NoGLError();
		}

		void Image::bind(int location) const
		{
			Assert(location >= 0);
			glActiveTexture(GL_TEXTURE0 + location); pwnAssert_NoGLError();
			glBindTexture(GL_TEXTURE_2D, text); pwnAssert_NoGLError();
		}

		unsigned int Image::getId() const
		{
			return text;
		}

		bool Image::IsSupported()
		{
			const bool multitexture = GLEW_ARB_multitexture == GL_TRUE;
			const bool mipmap = GLEW_VERSION_1_4 == GL_TRUE;
			return multitexture && mipmap;
		}

		Texture2::Texture2(core::IdPool* pool, bool useGlTexture)
			: useGlTexture(useGlTexture)
			, id(pool)
		{
		}

		Texture2::~Texture2()
		{
		}

		void Texture2::bind(int location) const
		{
			texture->bind(location);
		}

		const uint32 Texture2::sid() const
		{
			return id.value;
		}

		void Texture2::setImage(Image* img)
		{
			texture.reset( img );
		}

		void Load(Texture2* tex, uint32 width, uint32 height, const byte* pixels, const Engine& eng)
		{
			tex->setImage(new Image(true, width, height, pixels, true, GL_RGBA, eng.getAnistropy(), true));
		}
	}
}