#include <pwn/render/fbo.h>
#include "opengl_debug.hpp"
#include <pwn/core/str.h>

namespace pwn
{
	namespace render
	{
		RenderBuffer::RenderBuffer(int internalFormat, int width, int height)
			: buffer(0)
		{
			glGenRenderbuffers(1, &buffer);
			bind();
			glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
		}

		RenderBuffer::~RenderBuffer()
		{
			glDeleteRenderbuffers(1, &buffer);
		}

		void RenderBuffer::bind()
		{
			glBindRenderbuffer(GL_RENDERBUFFER, buffer);
		}

		unsigned int RenderBuffer::getBuffer() const	
		{
			return buffer;
		}

		namespace
		{
			void Bind(const Fbo* const fbo)
			{
				unsigned int ifbo = fbo ? fbo->getId() : 0;
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ifbo);
			}
		}

		Fbo::Fbo(int w, int h, bool mipmap)
			: fbo(0)
			, width(w)
			, height(h)
		{
			glGenFramebuffers(1, &fbo);
			Bind(this);

			depth.reset( new RenderBuffer(GL_DEPTH_COMPONENT, width, height));
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth->getBuffer());

			texture.reset( new Image(true, width, height, 0, mipmap, GL_RGBA) );
			const int mipmaplevel = 0;
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture->getId(), mipmaplevel);


			const int status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT) throw static_cast<string>(
				core::Str() << "Error when creating framebuffer: " << status);

			Bind(0);
		}

		void Fbo::bindTexture(int location)
		{
			texture->bind(location);
		}

		int Fbo::getWidth() const
		{
			return width;
		}

		int Fbo::getHeight() const
		{
			return height;
		}

		int Fbo::getId() const
		{
			return fbo;
		}

		bool Fbo::IsSupported()
		{
			return GLEW_ARB_framebuffer_object == GL_TRUE;
		}

		TextureUpdator::TextureUpdator(Fbo* fbo)
		{
			Bind(fbo);
			glPushAttrib(GL_VIEWPORT_BIT);
			glViewport(0, 0, fbo->getWidth(), fbo->getHeight());
		}

		TextureUpdator::~TextureUpdator()
		{
			glPopAttrib();
			Bind(0);
		}
	}
}
