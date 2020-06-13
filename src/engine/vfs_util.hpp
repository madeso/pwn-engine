#ifndef PWN_ENGINE_VFSUTIL_HPP
#define PWN_ENGINE_VFSUTIL_HPP

#include <pwn/number.h>

#include <physfs.h>
#include <boost/scoped_array.hpp>

namespace pwn
{
namespace engine
{
    class File
    {
    public:
        explicit File(PHYSFS_file* file);
        ~File();

        const std::size_t
        loadToMemory(boost::scoped_array<byte>* memory);

        PHYSFS_file* file;
    };
}
}

#endif
