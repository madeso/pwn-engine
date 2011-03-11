#ifndef PWN_CONVERT_MILKSHAPE_BINARY_HPP
#define PWN_CONVERT_MILKSHAPE_BINARY_HPP

#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Builder;
		class Animation;
	}

	namespace convert
	{
		namespace milkshape
		{
			namespace binary
			{
				void Read(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const string& meshpath);
			}
		}
	}
}

#endif
