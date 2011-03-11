#ifndef PWN_MESH_BUILDER
#define PWN_MESH_BUILDER

#include <pwn/number.h>
#include <pwn/math/types.h>

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
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

		uint32 NumberOfTriangles(const Mesh& mesh);

		class BPoint
		{
		public:
			BPoint(const math::vec3& location, BoneIndex bone);

			math::vec3 location;
			BoneIndex bone;
		};

		class BTriangle
		{
		public:
			typedef pwn::uint32 index;
			struct Vertex
			{
				index location;
				index texture;
				index normal;

				Vertex();
				Vertex(index vertex, index normal, index textureCoordinate);

				static Vertex Create_VN_T(index vertexAndNormal, index text);
			};
			Vertex v0;
			Vertex v1;
			Vertex v2;

			BTriangle(Vertex v0, Vertex v1, Vertex v2);
			//BTriangle(Vertex v[3]);
			BTriangle();

			Vertex& operator[](pwn::uint32 index);
			const Vertex& operator[](pwn::uint32 index) const;
		};

		class Builder
		{
		public:
			void clear();
			BTriangle::index addTextCoord(const math::vec2& v);
			BTriangle::index addPosition(const BPoint& pos);
			BTriangle::index addPosition(const math::vec3& pos, BoneIndex bone); // syntax sugar
			BTriangle::index addNormal(const math::vec3& norm);
			void addTriangle(pwn::uint32 material, const BTriangle& t);
			BTriangle::index addMaterial(Material m);

			void addQuad(bool reverse, pwn::uint32 material, const BTriangle::Vertex& v0, const BTriangle::Vertex& v1, const BTriangle::Vertex& v2, const BTriangle::Vertex& v3);
			void addFace(pwn::uint32 material, const std::vector<BTriangle::Vertex>& vertices);
			void addBone(const Bone& b);
			void setBox(Material material, real w, real h, real d, bool faceOut);
			void buildNormals();

			bool makeMesh(Mesh& mesh, Flatouter* flatouter) const;

			typedef std::vector<BTriangle> TriList;
			typedef std::map<pwn::uint32, TriList> TriMap;
			TriMap triangles;
			std::vector<BPoint> positions;
			std::vector<math::vec3> normals;
			std::vector<math::vec2> texcoords;
			std::vector<Bone> bones;
			std::vector<Material> materials;
		};
	}
}

#endif
