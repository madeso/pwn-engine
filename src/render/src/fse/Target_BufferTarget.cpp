#include <pwn/render/fse/Target_BufferTarget.h>
#include <pwn/render/fse/BufferReference.h>
#include <pwn/core/str.h>

namespace pwn
{
namespace render
{
    namespace fse
    {
        BufferTarget::BufferTarget(const core::Ptree& el)
            : width(el.get("width", 512))
            , height(el.get("height", 512))
        {
        }

        BufferTarget::~BufferTarget()
        {
        }

        void
        BufferTarget::apply(Provider& a, World3* world, const RenderArgs& ra)
        {
            buffer->updateTexture(a, world, ra);
        }

        void
        BufferTarget::link(Linker* usr)
        {
            buffer = createBuffer(getName(), width, height);
        }

        const string
        BufferTarget::getName() const
        {
            return getId();
        }

        string
        BufferTarget::toString() const
        {
            return core::Str()
                    << Target::toString() << " targeting a buffer (" << width
                    << "x" << height << ") named " << getName();
        }
    }
}
}
