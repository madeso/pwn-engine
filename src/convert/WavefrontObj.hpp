#ifndef PWN_CONVERT_WAVEFRONT_OBJ_HPP
#define PWN_CONVERT_WAVEFRONT_OBJ_HPP

#include <string>

namespace pwn
{
	namespace convert
	{
		class Converter;

		namespace obj
		{
			void read(Converter* converter, const std::string& file);
		}
	}
}

#endif