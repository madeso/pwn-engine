#include "vfs_util.hpp"
#include "pwn/core/str.h"

#include <fstream>
#include <vector>

namespace pwn
{
namespace engine
{
    const std::size_t
    loadFileToMemory(const std::string& filename, boost::scoped_array<byte>* memory)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if(!file.good())
        {
            throw (core::Str() << "Failed to open " << filename).toString();
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        memory->reset(new byte[size]);

        if (file.read(memory->get(), size))
        {
            /* worked! */
        }
        
        return size;
    }
}
}
