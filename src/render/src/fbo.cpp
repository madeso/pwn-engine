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
			glGenRenderbuffers(1, &buffer); pwnAssert_NoGLError();
			bind();
			glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height); pwnAssert_NoGLError();
		}

		RenderBuffer::~RenderBuffer()
		{
			glDeleteRenderbuffers(1, &buffer); pwnAssert_NoGLError();
		}

		void RenderBuffer::bind()
		{
			glBindRenderbuffer(GL_RENDERBUFFER, buffer); pwnAssert_NoGLError();
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
				glBindFramebuffer(GL_FRAMEBUFFER, ifbo); pwnAssert_NoGLError();
			}
		}

		Fbo::Fbo(int w, int h, bool mipmap)
			: fbo(0)
			, width(w)
			, height(h)
		{
			glGenFramebuffers(1, &fbo); pwnAssert_NoGLError();
			Bind(this);

			depth.reset( new RenderBuffer(GL_DEPTH_COMPONENT, width, height));
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth->getBuffer()); pwnAssert_NoGLError();

			// todo: investigate if anistropy should be used here to...? for now we just disable it
			texture.reset( new Image(true, width, height, 0, mipmap, GL_RGBA, 1, false) );
			const int mipmaplevel = 0;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), mipmaplevel); pwnAssert_NoGLError();


			const int status = glCheckFramebufferStatus(GL_FRAMEBUFFER); pwnAssert_NoGLError();
			if (status != GL_FRAMEBUFFER_COMPLETE) throw static_cast<string>(
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
			// return GLEW_ARB_framebuffer_object == GL_TRUE;
			return GLEW_VERSION_3_0 == GL_TRUE;
		}

		TextureUpdator::TextureUpdator(Fbo* fbo)
		{
			Bind(fbo);
			glPushAttrib(GL_VIEWPORT_BIT); pwnAssert_NoGLError();
			glViewport(0, 0, fbo->getWidth(), fbo->getHeight()); pwnAssert_NoGLError();
		}

		TextureUpdator::~TextureUpdator()
		{
			glPopAttrib(); pwnAssert_NoGLError();
			Bind(0);
		}
	}
}
