#ifndef PWN_CONVERT_3DS
#define PWN_CONVERT_3DS

#include <pwn/string.h>

namespace pwn
{
	namespace convert
	{
		class OptimizedMeshBuilder;
		namespace studio3ds
		{
			void read(OptimizedMeshBuilder* builder, const pwn::string& file);
		}
	}
}

#endif