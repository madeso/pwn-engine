#include <pwn/mesh/mesh.h>
#include <pwn/assert.h>

namespace pwn
{
	namespace mesh
	{
		Point::Point()
			: location(0, 0, 0)
			, bone(0)
		{
		}

		Point::Point(const math::vec3& alocation, BoneIndex abone)
			: location(alocation)
			, bone(abone)
		{
		}

		bool Point::hasBone() const
		{
			return bone != 0;
		}

		BoneIndex Point::getBone() const
		{
			Assert(bone != 0);
			return bone -1;
		}

		Mesh::Mesh()
		{
		}

		void Mesh::clear()
		{
			positions.clear();
			normals.clear();
			texcoords.clear();
			triangles.clear();
		}
	}
}
