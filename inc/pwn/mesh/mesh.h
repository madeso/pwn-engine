#ifndef PWN_MESH_MESH
#define PWN_MESH_MESH

#include <vector>
#include <map>
#include <pwn/math/types.h>
#include <pwn/mesh/triangle.h>

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace mesh
	{
		class Triangle;
		class Material;

		class Mesh
		{
		public:
			typedef std::vector<Triangle> TriangleList;
			typedef boost::shared_ptr<Material> MaterialPtr;

			Mesh();

			void clear();

			Triangle::index addPosition(const math::vec3& pos);
			Triangle::index addNormal(const math::vec3& norm);
			Triangle::index addTextCoord(const math::vec2& tc);

			void addTriangle(const Triangle& tri);
			Triangle::index addMaterial(MaterialPtr m);

			// todo: make private and add accessors instead...
			std::vector<math::vec3> positions;
			std::vector<math::vec3> normals;
			std::vector<math::vec2> texcoords;
			TriangleList triangles; // the map key referenses the materials vector below
			std::vector<MaterialPtr> materials; // this is done so materials could easily be swapped and overriden later on per actor
		};
	}
}

#endif