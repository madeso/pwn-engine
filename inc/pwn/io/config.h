#ifndef PWN_MESHIO_CONFIG_HPP
#define PWN_MESHIO_CONFIG_HPP

#include <pwn/core/config.h>
#include <pwn/string.h>

namespace pwn
{
	namespace meshio
	{
		/// loads a xml file from the global vfs to the tree
		void Load(pwn::core::Ptree* tree, const pwn::string& path);

		/// write the tree to a xml file in the vfs
		void Write(const pwn::core::Ptree& tree, const pwn::string& path);
	}
}

#endif