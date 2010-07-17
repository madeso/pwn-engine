#ifndef PWN_MESH_TRIANGLE
#define PWN_MESH_TRIANGLE

#include <pwn/number>

namespace pwn
{
	namespace mesh
	{
		class Triangle
		{
		public:
			typedef pwn::uint32 index;
			struct Vertex
			{
				index location;
				index normal;
				index texcoord;

				Vertex(index location, index normal, index texcoord);
				Vertex(index location_textcoord, index normal);
				Vertex();
			};
			Vertex v0;
			Vertex v1;
			Vertex v2;
			pwn::uint32 material;

			Triangle(pwn::uint32 material, Vertex v0, Vertex v1, Vertex v2);
			Triangle(pwn::uint32 material, Vertex v[3]);
			Triangle();

			Vertex& operator[](pwn::uint32 index);
			const Vertex& operator[](pwn::uint32 index) const;
		};
	}
}

#endif