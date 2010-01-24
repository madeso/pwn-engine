#include <pwn/mesh/mesh>
#include <pwn/mesh/triangle>

namespace pwn
{
	namespace mesh
	{
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

		Triangle::index Mesh::addPosition(const math::vec3& pos)
		{
			positions.push_back(pos);
			return static_cast<Triangle::index>(positions.size()-1);
		}

		Triangle::index Mesh::addNormal(const math::vec3& norm)
		{
			normals.push_back(norm);
			return static_cast<Triangle::index>(normals.size()-1);
		}

		Triangle::index Mesh::addTextCoord(const math::vec2& tc)
		{
			texcoords.push_back(tc);
			return static_cast<Triangle::index>(texcoords.size()-1);
		}

		void Mesh::addTriangle(const Triangle& tri)
		{
			triangles.push_back(tri);
		}

		Triangle::index Mesh::addMaterial(MaterialPtr m)
		{
			materials.push_back(m);
			return static_cast<Triangle::index>(materials.size()-1);
		}
	}
}