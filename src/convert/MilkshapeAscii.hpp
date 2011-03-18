#ifndef PWN_CONVERT_MILKSHAPE_ASCII_HPP
#define PWN_CONVERT_MILKSHAPE_ASCII_HPP

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
			namespace ascii
			{
				void Read(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& file);
			}
		}
	}
}

#endif
