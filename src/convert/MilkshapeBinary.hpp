#ifndef PWN_CONVERT_MILKSHAPE_BINARY_HPP
#define PWN_CONVERT_MILKSHAPE_BINARY_HPP

#include <pwn/string.h>

namespace pwn
{
	namespace convert
	{
		class OptimizedMeshBuilder;
		namespace milkshape
		{
			namespace binary
			{
				void Read(OptimizedMeshBuilder* builder, const string& meshpath);
			}
		}
	}
}

#endif
