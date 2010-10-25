#include <pwn/io/config.h>
#include <boost/property_tree/xml_parser.hpp>

#include "external/Physfs++.h"

namespace pwn
{
	namespace io
	{
		using namespace boost::property_tree::xml_parser;

		void Read(const pwn::string& path, pwn::core::Ptree* tree)
		{
			PhysFS::FileStream file(path.c_str(), PhysFS::OM_READ);
			read_xml(file, *tree);
		}

		void Write(const pwn::core::Ptree& tree, const pwn::string& path)
		{
			PhysFS::FileStream file(path.c_str(), PhysFS::OM_WRITE);
			write_xml(file, tree);
		}
	}
}
