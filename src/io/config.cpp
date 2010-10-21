#include <pwn/io/config.h>
#include <boost/property_tree/xml_parser.hpp>

#include "external/physfs_ifstream.h"
#include "external/physfs_ofstream.h"

namespace pwn
{
	namespace io
	{
		using namespace boost::property_tree::xml_parser;

		void Read(const pwn::string& path, pwn::core::Ptree* tree)
		{
			PHYSFS_ifstream file(path);
			read_xml(file, *tree);
		}

		void Write(const pwn::core::Ptree& tree, const pwn::string& path)
		{
			PHYSFS_ofstream file(path);
			write_xml(file, tree);
		}
	}
}
