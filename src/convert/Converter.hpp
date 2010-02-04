#ifndef PWN_CONVERT_CONVERTER_HPP
#define PWN_CONVERT_CONVERTER_HPP

#include <vector>
#include <map>

#include <pwn/math/types>
#include <pwn/math/rgba>
#include <pwn/string>
#include <pwn/mesh/Triangle>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
	};

	namespace convert
	{
		class OptimizedMeshBuilder
		{
		public:
			OptimizedMeshBuilder(::pwn::mesh::Mesh* mesh, bool optimzeNormals);
			
			mesh::Triangle::index addPosition(const math::vec3& pos);
			mesh::Triangle::index addNormal(const math::vec3& norm);
			mesh::Triangle::index addTextCoord(const math::vec2& tc);

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