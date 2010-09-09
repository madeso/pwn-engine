#ifndef PWN_MESHIO_IO
#define PWN_MESHIO_IO

#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
		class Animation;
	}

	namespace meshio
	{
		class WriteTarget
		{	
		public:
			explicit WriteTarget(const pwn::string& target);
			void set(const pwn::string& target);
			~WriteTarget();
		};

		void Write(const mesh::Mesh& mesh, const pwn::string& filename);
		void Read(mesh::Mesh* mesh, const pwn::string& filename);

		void Write(const mesh::Animation& animation, const pwn::string& filename);
		void Read(mesh::Animation* animation, const pwn::string& filename);
	}
}

#endif