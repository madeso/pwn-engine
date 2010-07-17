#ifndef PWN_MESH_BUILDER
#define PWN_MESH_BUILDER

#include <pwn/number.h>
#include <pwn/math/types.h>
#include <pwn/mesh/Triangle.h>

#include <boost/shared_ptr.hpp>
#include <vector>
#include <pwn/string.h>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
		class Material;

		Mesh* Move(Mesh* mesh, const math::vec3& dir);
		Mesh* Scale(Mesh* mesh, pwn::real scale);
		Mesh* SetBox(Mesh* mesh, boost::shared_ptr<Material> material, real width, real hight, real depth, bool faceOut);

		void AddQuad(Mesh* mesh, bool reverse, const pwn::uint32 material, const Triangle::Vertex& v0, const Triangle::Vertex& v1, const Triangle::Vertex& v2, const Triangle::Vertex& v3);
		Mesh* BuildNormals(Mesh* mesh);
		Mesh* InvertNormals(Mesh* mesh);

		void AddFace(Mesh* mesh, pwn::uint32 material, const std::vector<Triangle::Vertex>& vertices);

		void MoveTextures(Mesh* mesh, const pwn::string& newFolder);
	}
}

#endif