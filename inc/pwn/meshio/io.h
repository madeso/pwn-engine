#ifndef PWN_MESHIO_IO
#define PWN_MESHIO_IO

#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
	}

	namespace meshio
	{
		struct Compress
		{
			explicit Compress(bool all);

			bool materials;
			bool positions;
			bool normals;
			bool texcoords;
		};

		class WriteTarget
		{	
		public:
			explicit WriteTarget(const pwn::string& target);
			void set(const pwn::string& target);
			~WriteTarget();
		};

		void Write(const mesh::Mesh& mesh, const pwn::string& filename, const pwn::meshio::Compress compress);
		void Read(mesh::Mesh* mesh, const pwn::string& filename);
	}
}

#endif