#ifndef PWN_ENGINE_VFSUTIL_HPP
#define PWN_ENGINE_VFSUTIL_HPP

#include <string>

#include <pwn/number.h>

#include <boost/scoped_array.hpp>

namespace pwn
{
namespace engine
{
    const std::size_t
    loadFileToMemory(const std::string& filename, boost::scoped_array<byte>* memory);
}
}

#endif
