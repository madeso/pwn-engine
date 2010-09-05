#include "Converter.hpp"

#include <pwn/math/operations.h>
#include <pwn/mesh/Mesh.h>

#include <iostream>

namespace pwn
{
	namespace convert
	{
		OptimizedMeshBuilder::OptimizedMeshBuilder(bool optimizeNormals)
			: isBuilding(true)
			, mOptimizeNormals(optimizeNormals)
		{
		}

		mesh::Triangle::index OptimizedMeshBuilder::addPosition(const mesh::Point& pos)
		{
			if( isBuilding == false ) throw "done has been called...";
			return mBuilder.addPosition(pos);
		}

		uint32 OptimizedMeshBuilder::getMaterial(const pwn::string& name) const
		{
			for(uint32 id=0; id<mBuilder.materials.size(); ++id)
			{
				if( mBuilder.materials[id].name == name ) return id;
			}

			throw "unable to find material..";
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

		mesh::Triangle::index OptimizedMeshBuilder::addNormal(const pwn::math::vec3& n)
		{
			if( isBuilding == false ) throw "done has been called...";

			if( mOptimizeNormals )
			{
				const pwn::uint16 c = ::pwn::math::UnitVectorToCompressed(n);

				if( normalMap.find(c) == normalMap.end() )
				{
					normalMap[c] = mBuilder.addNormal(n);
				}

				normalConvertions.push_back(normalMap[c]);
				return normalMap[c];
			}
			else
			{
				return mBuilder.addNormal(n);
			}
		}

		mesh::Triangle::index OptimizedMeshBuilder::addTextCoord(const math::vec2& tc)
		{
			if( isBuilding == false ) throw "done has been called...";
			return mBuilder.addTextCoord(tc);
		}

		void OptimizedMeshBuilder::addTriangle(pwn::uint32 material, const mesh::Triangle& tri)
		{
			mBuilder.addTriangle(material, tri);
		}

		mesh::Triangle::index OptimizedMeshBuilder::addMaterial(mesh::Material m)
		{
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

			const bool optimizeNormals = mOptimizeNormals && mBuilder.normals.size() != 0;

			if( optimizeNormals == false ) return;

			
			const std::size_t mc = mBuilder.triangles.size();
			for(std::size_t mi=0; mi<mc; ++mi)
			{
				const std::size_t fc = mBuilder.triangles[mi].size();
				for(std::size_t i=0; i<fc; ++i)
				{
					for(int faceIndex=0; faceIndex<3; ++faceIndex)
					{
						::pwn::mesh::Triangle::Vertex& v = mBuilder.triangles[mi][i][faceIndex];
						if( optimizeNormals )
						{
							v.normal = normalConvertions[v.normal];
						}
					}
				}
			}
		}

		pwn::real OptimizedMeshBuilder::removedNormals() const
		{
			if( mOptimizeNormals ) return 0;
			const std::size_t normals = numberOfRemovedNormals();
			if( normals == 0 ) return 0;
			else return static_cast<pwn::real>(normals) / normalConvertions.size();
		}

		std::size_t OptimizedMeshBuilder::numberOfRemovedNormals() const
		{
			if( mOptimizeNormals ) return 0;
			return normalConvertions.size() - normalMap.size();
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

		const pwn::real CompressAndUncompress(pwn::real r)
		{
			return pwn::math::HalfToFloat(pwn::math::FloatToHalf(r));
		}

		const pwn::real LengthDiff(const pwn::math::vec3& v)
		{
			const pwn::math::vec3 loaded(CompressAndUncompress(v.x), CompressAndUncompress(v.y), CompressAndUncompress(v.z));
			return pwn::math::Abs(pwn::math::LengthOf(v-loaded) - pwn::math::LengthOf(v));
		}

		const pwn::real AngleDiff(const pwn::math::vec3& v)
		{
			const pwn::math::vec3 loaded = pwn::math::CompressedToUnitVector(pwn::math::UnitVectorToCompressed(v));
			const pwn::real a = pwn::math::AngleBetween(v, loaded).inDegrees();
			return a;
		}

		void EstimatedDataLossWhenCompressing(mesh::Mesh& data, Stat* positions, Stat* normals)
		{
			//std::cout << "Normal: " << data.normalMap.size() << ", " << data.normalConvertions.size() << std::endl;
			/* vertices */
			if( positions )
			{
				const std::size_t vc = data.positions.size();
				pwn::real min = 1000000000;
				pwn::real max = -1;
				pwn::real sum = 0;
				for(std::size_t i=0; i<vc; ++i)
				{
					const pwn::real d = LengthDiff(data.positions[i].location);
					sum += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				*positions = Stat(min, max, sum / vc);
			}

			if( normals )
			{
				const std::size_t nc = data.normals.size();
				pwn::real min = 10000000000;
				pwn::real max = -1;
				pwn::real sum = 0;
				for(std::size_t i=0; i<nc; ++i)
				{
					const pwn::real d = AngleDiff(data.normals[i]);
					sum += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				*normals = Stat(min, max, sum / nc);
			}
		}
	}
}