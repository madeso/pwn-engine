#ifndef PWN_CONVERT_3DS
#define PWN_CONVERT_3DS

#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Builder;
	}

	namespace convert
	{
		namespace studio3ds
		{
			void read(pwn::mesh::Builder* builder, const pwn::string& file);
		}
	}
}

#endif
