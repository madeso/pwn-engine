#ifndef PWN_CONVERT_3DS
#define PWN_CONVERT_3DS

#include <pwn/string.h>
#include "convert.hpp"

namespace pwn
{
namespace convert
{
    namespace studio3ds
    {
        void
        read(BuilderList* builders, const pwn::string& file);
    }
}
}

#endif
