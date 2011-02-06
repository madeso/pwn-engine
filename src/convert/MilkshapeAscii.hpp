#ifndef PWN_CONVERT_MILKSHAPE_ASCII_HPP
#define PWN_CONVERT_MILKSHAPE_ASCII_HPP

#include <pwn/string.h>

namespace pwn
{
	namespace convert
	{
		class OptimizedMeshBuilder;
		namespace milkshape
		{
			namespace ascii
			{
				void Read(OptimizedMeshBuilder* builder, const pwn::string& file);
			}
		}
	}
}

#endif
