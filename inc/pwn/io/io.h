#ifndef PWN_IO_IO
#define PWN_IO_IO

#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
		class Animation;
	}

	namespace io
	{
		void Write(const mesh::Mesh& mesh, const pwn::string& filename);
		void Read(mesh::Mesh* mesh, const pwn::string& filename);

		void Write(const mesh::Animation& animation, const pwn::string& filename);
		void Read(mesh::Animation* animation, const pwn::string& filename);
	}
}

#endif
