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

		class Builder;
		Builder CreateBox(Material material, real w, real h, real d, bool faceOut);

		class BPoint
		{
		public:
			BPoint(const math::vec3& position, math::vec4 bone);

			math::vec3 position;
			math::vec4 bone;
		};

		class BTriangle
		{
		public:
			typedef pwn::uint32 index;
			struct Vertex
			{
				index position;
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
			BTriangle::index addPosition(const math::vec3& pos, math::vec4 bone); // syntax sugar
			BTriangle::index addNormal(const math::vec3& norm);
			void addTriangle(pwn::uint32 material, const BTriangle& t);
			BTriangle::index addMaterial(Material m);

			BTriangle::index addMaterial(const pwn::string& name, mesh::Material m);
			uint32 getMaterial(const pwn::string& name) const;

			void addQuad(bool reverse, pwn::uint32 material, const BTriangle::Vertex& v0, const BTriangle::Vertex& v1, const BTriangle::Vertex& v2, const BTriangle::Vertex& v3);
			void addFace(pwn::uint32 material, const std::vector<BTriangle::Vertex>& vertices);

			void addBone(const Bone& b);

			void buildNormals();

			Mesh asMesh() const;

			typedef std::vector<BTriangle> TriList;
			typedef std::map<pwn::uint32, TriList> TriMap;
			TriMap triangles;
			std::vector<BPoint> positions;
			std::vector<math::vec3> normals;
			std::vector<math::vec2> texcoords;
			std::vector<Bone> bones;
			std::vector<Material> materials;

			typedef std::map<pwn::string, BTriangle::index> MaterialNameIdMap;
			MaterialNameIdMap materialid;
		};

		Builder* Move(Builder* mesh, const math::vec3& dir);
		Builder* Scale(Builder* mesh, pwn::real scale);
		Builder* InvertNormals(Builder* mesh);
		void MoveTextures(Builder* mesh, const pwn::string& newFolder);

		class Flatouter
		{
		public:
			explicit Flatouter(const Builder& mesh);
			void modify(Builder* mesh) const;
			void modify(Animation* animation) const;
		private:
			std::vector<BoneIndex> newIndices;
		};
	}
}

#endif
