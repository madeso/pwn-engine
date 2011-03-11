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
			OptimizedMeshBuilder();

			mesh::BTriangle::index addPosition(const mesh::BPoint& pos);
			mesh::BTriangle::index addNormal(const math::vec3& norm);
			mesh::BTriangle::index addTextCoord(const math::vec2& tc);

			// util functions, simply call theese on the mesh
			void addTriangle(pwn::uint32 material, const mesh::BTriangle& tri);
			mesh::BTriangle::index addMaterial(const pwn::string& name, mesh::Material m);
			void addBone(const ::pwn::mesh::Bone& b);

			void done();

			uint32 getMaterial(const pwn::string& name) const;

			::pwn::mesh::Builder mBuilder;
			::pwn::mesh::Animation mAnimation;
		private:
			bool isBuilding;

			typedef std::map<pwn::string, uint32> MaterialNameIdMap;
			MaterialNameIdMap materialid;
		};
	}
}

#endif
