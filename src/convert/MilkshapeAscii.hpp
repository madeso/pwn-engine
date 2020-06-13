#ifndef PWN_CONVERT_MILKSHAPE_ASCII_HPP
#define PWN_CONVERT_MILKSHAPE_ASCII_HPP

#include <pwn/string.h>
#include "convert.hpp"

namespace pwn
{
namespace mesh
{
    class Animation;
}

namespace convert
{
    namespace milkshape
    {
        namespace ascii
        {
            void
            Read(BuilderList* builders, const pwn::string& file);
        }
    }
}
}

#endif
