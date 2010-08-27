#ifndef PWN_CONVERT_CONVERTER_HPP
#define PWN_CONVERT_CONVERTER_HPP

#include <vector>
#include <map>

#include <pwn/math/types.h>
#include <pwn/math/rgba.h>
#include <pwn/string.h>
#include <pwn/mesh/Triangle.h>
#include <pwn/mesh/Mesh.h>

namespace pwn
{
	namespace convert
	{
		class OptimizedMeshBuilder
		{
		public:
			OptimizedMeshBuilder(::pwn::mesh::Mesh* mesh, bool optimzeNormals);
			
			mesh::Triangle::index addPosition(const mesh::Point& pos);
			mesh::Triangle::index addNormal(const math::vec3& norm);
			mesh::Triangle::index addTextCoord(const math::vec2& tc);

			// util functions, simply call theese on the mesh
			void addTriangle(const mesh::Triangle& tri);
			mesh::Triangle::index addMaterial(mesh::Mesh::MaterialPtr m);
			void addBone(const ::pwn::mesh::Bone& b);

			void done();

			::pwn::mesh::Mesh* mesh();

			uint32 getMaterial(const pwn::string& name) const;

			uint32 validate() const;

			// stat functions
			pwn::real removedNormals() const;
			std::size_t numberOfRemovedNormals() const;
		private:
			typedef mesh::Triangle::index NormalIndex;
			typedef pwn::uint16 CompressedNormal;

			bool isBuilding;

			::pwn::mesh::Mesh* mMesh;
			bool mOptimizeNormals;
			std::map<CompressedNormal, NormalIndex> normalMap;
			std::vector<NormalIndex> normalConvertions;
		};

		struct Stat
		{
			Stat();
			Stat(pwn::real aMin, pwn::real aMax, pwn::real aAverage);

			pwn::real min;
			pwn::real max;
			pwn::real average;
		};

		void EstimatedDataLossWhenCompressing(mesh::Mesh& data, Stat* positions, Stat* normals);
	}
}

#endif