#ifndef PWN_RENDER_MESH
#define PWN_RENDER_MESH

#include <pwn/math/types>
#include <boost/smart_ptr/scoped_array.hpp>

namespace pwn
{
	namespace render
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
			};
			Vertex vertices[3];
		};

		class Mesh
		{
		public:
			boost::scoped_array<math::vec3> positions;
			boost::scoped_array<math::vec3> normals;
			boost::scoped_array<math::vec2> texcoords;
			boost::scoped_array<Triangle> triangles;
		};
	}
}

#endif