#ifndef PWN_CONVERT_MILKSHAPE_COMMON_HPP
#define PWN_CONVERT_MILKSHAPE_COMMON_HPP

#include <vector>
#include <pwn/string.h>
#include <pwn/math/types.h>
#include <boost/utility.hpp>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
		class Animation;
	}

	namespace convert
	{
		class OptimizedMeshBuilder;
		namespace milkshape
		{
			namespace common
			{
				class Mesh;
				class Material;
				class Bone;
				class Vertex;
				class Normal;
				class Tri;

				class Model
				{
				public:
					Model();

					int framecount;
					real currentFrame;

					std::vector<Mesh> meshes;
					std::vector<Material> materials;
					std::vector<Bone> bones;

					Mesh& newMesh();
					Material& newMaterial();
					Bone& newBone();
					int boneId(string bone);
					void mapBonesToId();
				};

				class Mesh
				{
				public:
					Mesh();

					string name;
					int flags;
					int materialId;

					std::vector<Vertex> vertices;
					std::vector<Normal> normals;
					std::vector<Tri> tris;

					Vertex& newVertex();
					Normal& newNormal();
					Tri& newTri();
				};

				class Vertex
				{
				public:
					Vertex();

					int flags;
					math::vec2 uv;
					math::vec3 pos;
					int bone;
				};

				class Normal
				{
				public:
					Normal();

					math::vec3 norm;

					void normalize();
				};

				class Tri
				{
				public:
					int flags;
					int v1;
					int v2;
					int v3;
					int n1;
					int n2;
					int n3;
					int smoothingGroup;

					Tri();
					void buildNormal(Mesh mesh);
				};

				class Color
				{
				public:
					real r;
					real g;
					real b;
					real a;

					Color();

					void parse(string p);
				};

				class Material
				{
				public:
					string name;
					Color ambient;
					Color diffuse;
					Color specular;
					Color emissive;
					real shininess;
					real transperency;
					string diffuseTexture;
					string alphatexture;

					Material();
				};

				class PositionKey
				{
				public:
					real time;
					real x;
					real y;
					real z;
				};

				class RotationKey
				{
				public:
					real time;
					real x;
					real y;
					real z;
				};

				class Bone
				{
				public:
					Bone();

					string name;
					string parentName;
					int parentId;
					int flags;
					real x;
					real y;
					real z;
					real rx;
					real ry;
					real rz;

					std::vector<PositionKey> positions;
					std::vector<RotationKey> rotations;

					RotationKey& newRotationKey();
					PositionKey& newPositionKey();
				};

				namespace MilkshapeCommon
				{
					mesh::Animation ExtractAnimation(const Model& model);

					void ExtractMeshDefinition(const Model& model, OptimizedMeshBuilder* builder);

					math::quat makeQuat(const math::vec3& angles);

					const string SmartTexture(const string& p);
				}
			}
		}
	}
}

#endif
