#include <pwn/render/fse/BufferReference.h>
#include <pwn/render/fse/exceptions.h>
#include <pwn/render/fse/Provider.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			BufferReference::BufferReference(const string& aname)
				: name(aname)
			{
			}

			BufferReference::~BufferReference()
			{
			}
			
			const string& BufferReference::getName() const
			{
				return name;
			}

			void BufferReference::setBuffer(FboPtr fbo)
			{
				if (buffer.get() != 0) throw FseException("Error, buffer already initialized!");
				buffer = fbo;
			}
			
			void BufferReference::bindTexture(int location)
			{
				buffer->bindTexture(location);
			}
			
			void BufferReference::updateTexture(Provider& a, World3* world, const RenderArgs& r)
			{
				RenderArgs ra(r, getWidth(), getHeight());
				TextureUpdator up(buffer.get());
				a.doProvide(world, ra);
			}
			
			int BufferReference::getWidth() const
			{
				return buffer->getWidth();
			}
			
			int BufferReference::getHeight() const
			{
				return buffer->getHeight();
			}
		}
	}
}
