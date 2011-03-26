#ifndef PWN_CONVERT_AN8_HPP
#define PWN_CONVERT_AN8_HPP

#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Builder;
	}

	namespace convert
	{
		namespace an8
		{
			void read(pwn::mesh::Builder* builder, const pwn::string& file);
		}
	}
}

#endif
