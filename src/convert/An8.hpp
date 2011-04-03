#ifndef PWN_CONVERT_AN8_HPP
#define PWN_CONVERT_AN8_HPP

#include <vector>
#include <pwn/string.h>
#include "convert.hpp"

namespace pwn
{
	namespace convert
	{
		namespace an8
		{
			void read(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& file);
		}
	}
}

#endif
