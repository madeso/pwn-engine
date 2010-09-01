#ifndef PWN_MESH_BUILDER
#define PWN_MESH_BUILDER

#include <pwn/number.h>
#include <pwn/math/types.h>

#include <boost/shared_ptr.hpp>
#include <vector>
#include <pwn/string.h>
#include <pwn/mesh/mesh.h>

namespace pwn
{
	namespace mesh
	{
		class Mesh;

		Mesh* Move(Mesh* mesh, const math::vec3& dir);
		Mesh* Scale(Mesh* mesh, pwn::real scale);
		Mesh* InvertNormals(Mesh* mesh);
		void MoveTextures(Mesh* mesh, const pwn::string& newFolder);

		class Builder
		{
			void clear();
			Triangle::index addTextCoord(const math::vec2& v);
			Triangle::index addPosition(const math::vec3& v, Triangle::index bone);
			void addTriangle(pwn::uint32 material, const Triangle& t);
			void addQuad(bool reverse, pwn::uint32 material, const Triangle::Vertex& v0, const Triangle::Vertex& v1, const Triangle::Vertex& v2, const Triangle::Vertex& v3);
			void addFace(pwn::uint32 material, const std::vector<Triangle::Vertex>& vertices);
			void setBox(boost::shared_ptr<Material> material, real w, real h, real d, bool faceOut);
			void buildNormals();
			
			std::vector<Triangle> triangles;
			std::vector<Point> positions;
			std::vector<math::vec3> normals;
			std::vector<math::vec2> texcoords;
			std::vector<Bone> bones;
			std::vector<boost::shared_ptr<Material> > materials;
		};
	}
}

#endif