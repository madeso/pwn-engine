#include <pwn/mesh/mesh.h>
#include <pwn/assert.h>
#include <boost/foreach.hpp>
#include <pwn/assert.h>

namespace pwn
{
	namespace mesh
	{
		Point::Point()
		{
		}

		Point::Point(const math::vec3& alocation, math::vec4 abone)
			: position(alocation)
			, bone(abone)
		{
		}

		Point::Point(const math::vec3& alocation, math::vec3 anormal, math::vec2 atextcoord, math::vec4 abone)
			: position(alocation)
			, normal(anormal)
			, textcoord(atextcoord)
			, bone(abone)
		{
		}

		bool Point::hasBone() const
		{
			return bone[0] >= 0.0f;
		}

		math::vec4 Point::getBone() const
		{
			return bone;
		}

		int Check(const string& reason, size_t index, size_t size)
		{
			if( index >= size )
			{
				Assert( 0 && reason.c_str());
				return 1;
			}
			else return 0;
		}


		void SetBoneIndex(real* val, BoneIndex newIndex)
		{
			const real inf = GetBoneInfluence(*val);
			*val = newIndex + inf;
		}

		BoneIndex GetBoneIndex(real val)
		{
			BoneIndex r = static_cast<int>(floor(val));
			Assert(r >= 0);
			return r;
		}

		real GetBoneInfluence(real val)
		{
			Assert(val > 0);
			return val - floor(val);
		}




		Mesh::Mesh()
		{
		}

		Mesh::Mesh(const std::vector<math::vec3>& posv,
				const std::vector<math::vec3>& normv,
				const std::vector<math::vec2>&textv,
				const std::vector<math::vec4>& bonev,
				const TriangleMap& trim,
				const std::vector<Bone>& abones,
				const std::vector<Material>& amaterials)
				: vertexes(posv, normv, textv, bonev)
				, bones(abones)
				, triangles(trim)
				, materials(amaterials)

		{
		}

		pwn::uint32 Mesh::validate(bool testSortedBones) const
		{
			pwn::uint32 errors = 0;

			// test if bones are sorted
			if( testSortedBones )
			{
				for(std::size_t i = 0; i<bones.size(); ++i)
				{
					if( bones[i].hasParent() )
					{
						errors += Check("bone sorted", bones[i].getParent(), i );
					}
				}
			}

			// test that the triangles point to valid indices
			const std::size_t materialSize = materials.size();
			BOOST_FOREACH(const TriangleMap::value_type& mt, triangles)
			{
				errors += Check("material index", mt.first, materialSize);
				BOOST_FOREACH(const pwn::mesh::Triangle& t, mt.second)
				{
					for(int i=0; i<3; ++i)
					{
						errors += Check("position index", t[i], vertexes.getCount());
					}
				}
			}

			// test that the points are valid
			const std::size_t boneSize = bones.size();
			for(pwn::uint32 i = 0; i < vertexes.getCount(); ++i)
			{
				for(int b=0; b<4; ++b)
				{
					const real x = vertexes.getBone(i)[b];
					if( x >= 0 )
					{
						const BoneIndex p = GetBoneIndex(x);
						errors += Check("bone index", p -1, boneSize);
					}
				}
			}

			return errors;
		}

		const VertexData& Mesh::data() const
		{
			return vertexes;
		}

		const std::vector<Material>& Mesh::getMaterials() const
		{
			return materials;
		}

		const std::vector<Bone>& Mesh::getBones() const
		{
			return bones;
		}

		const Mesh::TriangleMap& Mesh::getTriangles() const
		{
			return triangles;
		}

		void Mesh::setLocationNormal(const uint32 i, const math::vec3& pos, const math::vec3& norm)
		{
			vertexes.setLocationNormal(i, pos, norm);
		}

		pwn::uint32 Mesh::getNumberOfTriangles() const
		{
			uint32 count = 0;
			for(Mesh::TriangleMap::const_iterator i = triangles.begin(); i != triangles.end(); ++i)
			{
				count += i->second.size();
			}
			return count;
		}




		VertexData::VertexData()
		{
		}

		VertexData::VertexData(const std::vector<math::vec3>& posv,
				const std::vector<math::vec3>& normv,
				const std::vector<math::vec2>&textv,
				const std::vector<math::vec4>& bonev)
				: count(posv.size())
		{
			Assert(posv.size() == normv.size() && textv.size() == bonev.size() && normv.size() == textv.size()); // if this fails, we didnt get a matched data
			locations.reset(new real[count*3]);
			normals.reset(new real[count*3]);
			textcoords.reset(new real[count*2]);
			boneindexes.reset(new real[count*4]);
			for(uint32 i=0; i<count; ++i)
			{
				pos(i) = posv[i];
				norm(i) = normv[i];
				tex(i) = textv[i];
				bone(i) = bonev[i];
			}
		}

		VertexData::VertexData(const VertexData& m)
		{
			doCopy(m);
		}

		const VertexData& VertexData::operator=(const VertexData& m)
		{
			doCopy(m);
			return *this;
		}

		pwn::uint32 VertexData::getCount() const
		{
			return count;
		}

		math::vec4 VertexData::getBone(pwn::uint32 id) const
		{
			return bone(id);
		}

		const Point VertexData::getPoint(uint32 i) const
		{
			return Point(pos(i), norm(i), tex(i), bone(i));
		}

		void VertexData::setLocationNormal(const uint32 i, const math::vec3& apos, const math::vec3& anorm)
		{
			pos(i) = apos;
			norm(i) = anorm;
		}

		math::vec3x VertexData::pos(const uint32 i)
		{
			return math::vec3x(&locations[i*3]);
		}

		math::vec3x VertexData::norm(const uint32 i)
		{
			return math::vec3x(&normals[i*3]);
		}

		math::vec2x VertexData::tex(const uint32 i)
		{
			return math::vec2x(&textcoords[i*2]);
		}

		math::vec4x VertexData::bone(const uint32 i)
		{
			return math::vec4x(&boneindexes[i*4]);
		}

		const math::vec3x VertexData::pos(const uint32 i) const
		{
			return math::vec3x(&locations[i*3]);
		}

		const math::vec3x VertexData::norm(const uint32 i) const
		{
			return math::vec3x(&normals[i*3]);
		}

		const math::vec2x VertexData::tex(const uint32 i) const
		{
			return math::vec2x(&textcoords[i*2]);
		}

		const math::vec4x VertexData::bone(const uint32 i) const
		{
			return math::vec4x(&boneindexes[i*4]);
		}

		template<typename T>
		void Copy(boost::scoped_array<T>& dst, const boost::scoped_array<T>& src, std::size_t count)
		{
			dst.reset(new T[count]);
			memcpy(dst.get(), src.get(), sizeof(T)*count);
		}

		void VertexData::doCopy(const VertexData& m)
		{
			count = m.count;

			Copy<real>(locations, m.locations, 3*count);
			Copy<real>(normals, m.normals, 3*count);
			Copy<real>(textcoords, m.textcoords, 2*count);
			Copy<real>(boneindexes, m.boneindexes, 4*count);
		}
	}
}
