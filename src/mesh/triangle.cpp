#include <pwn/mesh/triangle>

namespace pwn
{
	namespace mesh
	{
		Triangle::Vertex::Vertex(index location, index normal, index texcoord)
			: location(location)
			, normal(normal)
			, texcoord(texcoord)
		{
		}

		Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2)
			: v0(v0)
			, v1(v1)
			, v2(v2)
		{
		}

		Triangle::Triangle(Vertex v[3])
			: v0(v[0])
			, v1(v[1])
			, v2(v[2])
		{
		}

		namespace // local
		{
			Triangle::Vertex Triangle::* const offsets[3] = {&Triangle::v0, &Triangle::v1, &Triangle::v2};
		}

		Triangle::Vertex& Triangle::operator[](pwn::uint32 index)
		{
			return this->*offsets[index];
		}

		const Triangle::Vertex& Triangle::operator[](pwn::uint32 index) const
		{
			return this->*offsets[index];
		}
	}
}