#ifndef PWN_CONVERT_WRITER_HPP
#define PWN_CONVERT_WRITER_HPP

#include <pwn/string>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
	}
}

namespace pwn
{
	namespace convert
	{
		void Write(mesh::Mesh& data, const pwn::string& file, bool optimize);
		void Test(mesh::Mesh& data);
	}
}

#endif