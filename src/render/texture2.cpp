#include <pwn/render/texture2>

#include <SFML/OpenGl.hpp>

namespace pwn
{
	namespace render
	{
		Texture2::Texture2(core::IdPool* pool)
			: texture(0)
			, id(pool)
		{
			glGenTextures( 1, &texture );
		}

		Texture2::~Texture2()
		{
			glDeleteTextures( 1, &texture );
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
			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
		}
	}
}