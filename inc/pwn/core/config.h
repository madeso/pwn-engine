#ifndef PWN_CORE_CONFIG
#define PWN_CORE_CONFIG

#include <boost/property_tree/ptree.hpp>
#include <pwn/string.h>

namespace pwn
{
	namespace core
	{
		typedef boost::property_tree::basic_ptree<string, string> Ptree;
	}
}

#endif
