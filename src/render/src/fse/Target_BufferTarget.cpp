#include <pwn/render/fse/Target_BufferTarget.h>
#include <pwn/render/fse/BufferReference.h>
#include <pwn/core/Str.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			BufferTarget::BufferTarget(const core::Ptree& el)
				: width( el.get("width", 512) )
				, height( el.get("height", 512) )
			{
			}

			BufferTarget::~BufferTarget()
			{
			}

			void BufferTarget::apply(Provider& a, const RenderArgs& ra)
			{
				buffer->updateTexture(a, ra);
			}

			int BufferTarget::getWidth()
			{
				return buffer->getWidth();
			}

			int BufferTarget::getHeight()
			{
				return buffer->getHeight();
			}

			void BufferTarget::link(Linker* usr)
			{
				buffer = createBuffer(getName(), width, height);
			}

			const string BufferTarget::getName() const
			{
				return getId();
			}

			string BufferTarget::toString() const
			{
				return core::Str() << Target::toString() << " targeting a buffer (" << width << "x" << height << ") named " << getName();
			}
		}
	}
}
