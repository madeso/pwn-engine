#include <pwn/io/io.h>

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include "vfs.hpp"
#include <boost/foreach.hpp>
#include <map>

namespace pwn
{
namespace io
{
    const pwn::uint8 kVersion = 1;

    template <class AnimationArg, typename VersionType>
    class AnimationFile
    {
    public:
        template <class Filer>
        static void
        handle(Filer& vf, AnimationArg animation, VersionType version)
        {
            vf.handle8(version);
            if (version != kVersion)
            {
                throw "animation version mismatch";
            }
            vf.handleReal(animation.length);

            pwn::uint32 size = vf.handleVectorSize(animation.bones);
            for (pwn::uint32 i = 0; i < size; ++i)
            {
                vf.handleVector(animation.bones[i].fp);
                vf.handleVector(animation.bones[i].fr);
                if (animation.bones[i].fp.empty())
                {
                    throw "invalid position data";
                }
                if (animation.bones[i].fr.empty())
                {
                    throw "invalid rotation data";
                }
            }
        }
    };

    void
    Write(const mesh::Animation& animation, const pwn::string& filename)
    {
        VirtualFile vf(filename, false);
        FileWriter w;
        w.file = &vf;
        AnimationFile<const mesh::Animation&, const pwn::uint8>::handle(
                w,
                animation,
                kVersion);
    }

    void
    Read(mesh::Animation* animation, const pwn::string& filename)
    {
        pwn::uint8 version = kVersion;
        VirtualFile vf(filename, true);
        FileReader r;
        r.file = &vf;
        AnimationFile<mesh::Animation&, pwn::uint8&>::handle<>(
                r,
                *animation,
                version);
    }
}
}
