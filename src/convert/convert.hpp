#ifndef PWN_CONVERT_HPP
#define PWN_CONVERT_HPP

#include <vector>
#include <pwn/string.h>
#include <pwn/mesh/builder.h>

namespace pwn
{
	namespace convert
	{
		struct Entry
		{
			Entry(const mesh::Builder& b, const string& n="");
			pwn::mesh::Builder builder;
			pwn::string name;
		};

		typedef std::vector<Entry> BuilderList;
	}
}

#endif
