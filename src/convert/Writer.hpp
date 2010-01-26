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
		struct Compress
		{
			explicit Compress(bool all);

			bool materials;
			bool positions;
			bool normals;
			bool texcoords;
		};
		void Write(mesh::Mesh& data, const pwn::string& file, const Compress compress);
		void Test(mesh::Mesh& data);
	}
}

#endif