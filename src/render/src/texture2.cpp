#include <pwn/render/texture2.h>

#include <SFML/OpenGl.hpp>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

namespace pwn
{
	namespace render
	{
		Texture2::Texture2(core::IdPool* pool, bool useGlTexture)
			: useGlTexture(useGlTexture)
			, texture(0)
			, id(pool)
		{
			if( useGlTexture )
			{
				glGenTextures( 1, &texture );
			}
		}

		Texture2::~Texture2()
		{
			if( useGlTexture )
			{
				glDeleteTextures( 1, &texture );
			}
		}

		const uint32 Texture2::tid() const
		{
			return texture;
		}

		const uint32 Texture2::sid() const
		{
			return id.value;
		}

		void Load(Texture2* tex, uint32 width, uint32 height, const byte* pixels)
		{
			glBindTexture(GL_TEXTURE_2D, tex->tid());
			// todo: setup constants
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
		}
	}
}