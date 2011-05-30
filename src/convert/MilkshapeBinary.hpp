#ifndef PWN_CONVERT_MILKSHAPE_BINARY_HPP
#define PWN_CONVERT_MILKSHAPE_BINARY_HPP

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
			namespace binary
			{
				void Read(BuilderList* builders, const string& meshpath);
			}
		}
	}
}

#endif
