#ifdef _MSC_VER
// boost_1_43_0\boost/iostreams/detail/streambuf/indirect_streambuf.hpp(248) : warning C4996:
// 'std::char_traits<char>::move': Function call with parameters that may be unsafe - this call relies on
// the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
// See documentation on how to use Visual C++ 'Checked Iterators'
#define _SCL_SECURE_NO_WARNINGS
#endif

#include <pwn/io/config.h>
#include <boost/property_tree/xml_parser.hpp>

#include "PhysFS++.h"

namespace pwn
{
namespace io
{
    using namespace boost::property_tree::xml_parser;

    void
    Read(const pwn::string& path, pwn::core::Ptree* tree)
    {
        PhysFS::FileStream file(path.c_str(), PhysFS::OM_READ);
        read_xml(file, *tree);
    }

    void
    Write(const pwn::core::Ptree& tree, const pwn::string& path)
    {
        PhysFS::FileStream file(path.c_str(), PhysFS::OM_WRITE);
        write_xml(file, tree);
    }
}
}
