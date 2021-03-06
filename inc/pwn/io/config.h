#ifndef PWN_IO_CONFIG_HPP
#define PWN_IO_CONFIG_HPP

#include <pwn/core/config.h>
#include <pwn/string.h>

namespace pwn
{
	namespace io
	{
		/// loads a xml file from the global vfs to the tree
		void Read(const pwn::string& path, pwn::core::Ptree* tree);

		/// write the tree to a xml file in the vfs
		void Write(const pwn::core::Ptree& tree, const pwn::string& path);
	}
}

#endif
