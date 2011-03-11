#include "Converter.hpp"

#include <pwn/math/operations.h>
#include <pwn/mesh/mesh.h>
#include <pwn/assert.h>

#include <iostream>

namespace pwn
{
	namespace convert
	{
		OptimizedMeshBuilder::OptimizedMeshBuilder()
			: isBuilding(true)
		{
		}

		mesh::BTriangle::index OptimizedMeshBuilder::addPosition(const mesh::BPoint& pos)
		{
			if( isBuilding == false ) throw "done has been called...";
			return mBuilder.addPosition(pos);
		}

		uint32 OptimizedMeshBuilder::getMaterial(const pwn::string& name) const
		{
			MaterialNameIdMap::const_iterator res = materialid.find(name);
			if( res == materialid.end() ) throw "unable to find material..";
			else return res->second;
		}

		uint32 Check(const pwn::string& name, std::size_t i, std::size_t max)
		{
			if( i >= max )
			{
				std::cerr << name << " is bad.. " << i << " of " << max << std::endl;
				return 1;
			}
			else return 0;
		}

		mesh::BTriangle::index OptimizedMeshBuilder::addNormal(const pwn::math::vec3& n)
		{
			if( isBuilding == false ) throw "done has been called...";

			return mBuilder.addNormal(n);
		}

		mesh::BTriangle::index OptimizedMeshBuilder::addTextCoord(const math::vec2& tc)
		{
			if( isBuilding == false ) throw "done has been called...";
			Assert( math::IsWithinInclusive(0, math::X(tc), 1) );
			Assert( math::IsWithinInclusive(0, math::Y(tc), 1) );
			return mBuilder.addTextCoord(tc);
		}

		void OptimizedMeshBuilder::addTriangle(pwn::uint32 material, const mesh::BTriangle& tri)
		{
			mBuilder.addTriangle(material, tri);
		}

		mesh::BTriangle::index OptimizedMeshBuilder::addMaterial(const pwn::string& name, mesh::Material m)
		{
			materialid[name] = mBuilder.materials.size();
			return mBuilder.addMaterial(m);
		}

		void OptimizedMeshBuilder::addBone(const ::pwn::mesh::Bone& b)
		{
			mBuilder.bones.push_back(b);
		}

		void OptimizedMeshBuilder::done()
		{
			if( isBuilding == false ) throw "done has been called...";
			isBuilding = false;
		}

		Stat::Stat()
			: min(0)
			, max(0)
			, average(0)
		{
		}

		Stat::Stat(pwn::real aMin, pwn::real aMax, pwn::real aAverage)
			: min(aMin)
			, max(aMax)
			, average(aAverage)
		{
		}

		const pwn::real AngleDiff(const pwn::math::vec3& v)
		{
			const pwn::math::vec3 loaded = pwn::math::CompressedToUnitVector(pwn::math::UnitVectorToCompressed(v));
			const pwn::real a = pwn::math::AngleBetween(v, loaded).inDegrees();
			return a;
		}

		void EstimatedDataLossWhenCompressing(mesh::Mesh& data, Stat* normals)
		{
			if( normals )
			{
				const std::size_t nc = data.positions.size();
				pwn::real min = 10000000000;
				pwn::real max = -1;
				pwn::real sum = 0;
				for(std::size_t i=0; i<nc; ++i)
				{
					const pwn::real d = AngleDiff(data.positions[i].normal);
					sum += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				*normals = Stat(min, max, sum / nc);
			}
		}
	}
}
