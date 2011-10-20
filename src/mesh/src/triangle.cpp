#include <pwn/mesh/mesh.h>

namespace pwn
{
	namespace mesh
	{
		Triangle::Triangle(VertexIndex v0, VertexIndex v1, VertexIndex v2)
			: v0(v0)
			, v1(v1)
			, v2(v2)
		{
		}

		Triangle::Triangle(VertexIndex v[3])
			: v0(v[0])
			, v1(v[1])
			, v2(v[2])
		{
		}

		Triangle::Triangle()
		{
		}

		namespace // local
		{
			Triangle::VertexIndex Triangle::* const offsets[3] = {&Triangle::v0, &Triangle::v1, &Triangle::v2};
		}

		Triangle::VertexIndex& Triangle::operator[](pwn::uint32 index)
		{
			return this->*offsets[index];
		}

		const Triangle::VertexIndex& Triangle::operator[](pwn::uint32 index) const
		{
			return this->*offsets[index];
		}
	}
}
