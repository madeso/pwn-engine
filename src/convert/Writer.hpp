#ifndef PWN_CONVERT_WRITER_HPP
#define PWN_CONVERT_WRITER_HPP

#include <pwn/string>

namespace pwn
{
	namespace convert
	{
		class Converter;
	}
}

namespace pwn
{
	namespace convert
	{
		void Write(Converter& data, const pwn::string& file, bool optimize);
		void Test(Converter& data);
	}
}

#endif