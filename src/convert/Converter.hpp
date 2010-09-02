#ifndef PWN_CONVERT_CONVERTER_HPP
#define PWN_CONVERT_CONVERTER_HPP

#include <vector>
#include <map>

#include <pwn/math/types.h>
#include <pwn/math/rgba.h>
#include <pwn/string.h>
#include <pwn/mesh/builder.h>

namespace pwn
{
	namespace convert
	{
		class OptimizedMeshBuilder
		{
		public:
			OptimizedMeshBuilder(bool optimzeNormals);
			
			mesh::Triangle::index addPosition(const mesh::Point& pos);
			mesh::Triangle::index addNormal(const math::vec3& norm);
			mesh::Triangle::index addTextCoord(const math::vec2& tc);

			// util functions, simply call theese on the mesh
			void addTriangle(pwn::uint32 material, const mesh::Triangle& tri);
			mesh::Triangle::index addMaterial(mesh::Mesh::MaterialPtr m);
			void addBone(const ::pwn::mesh::Bone& b);

			void done();

			::pwn::mesh::Mesh* mesh();

			uint32 getMaterial(const pwn::string& name) const;

			// stat functions
			pwn::real removedNormals() const;
			std::size_t numberOfRemovedNormals() const;

			::pwn::mesh::Builder mBuilder;
		private:
			typedef mesh::Triangle::index NormalIndex;
			typedef pwn::uint16 CompressedNormal;

			bool isBuilding;

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