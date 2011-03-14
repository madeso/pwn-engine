#include <pwn/mesh/mesh.h>
#include <pwn/assert.h>
#include <boost/foreach.hpp>

namespace pwn
{
	namespace mesh
	{
		Point::Point()
			: location(0, 0, 0)
			, bone(0)
		{
		}

		Point::Point(const math::vec3& alocation, BoneIndex abone)
			: location(alocation)
			, bone(abone)
		{
		}

		Point::Point(const math::vec3& alocation, math::vec3 anormal, math::vec2 atextcoord, BoneIndex abone)
			: location(alocation)
			, normal(anormal)
			, textcoord(atextcoord)
			, bone(abone)
		{
		}

		bool Point::hasBone() const
		{
			return bone != 0;
		}

		BoneIndex Point::getBone() const
		{
			Assert(bone != 0);
			return bone -1;
		}

		Mesh::Mesh()
		{
		}

		Mesh::Mesh(const Mesh& m)
		{
			doCopy(m);
		}

		const Mesh& Mesh::operator=(const Mesh& m)
		{
			doCopy(m);
			return *this;
		}
		
		pwn::uint32 Mesh::getCount() const
		{
			return count;
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
						errors += Check("position index", t[i], count);
					}
				}
			}

			// test that the points are valid
			const std::size_t boneSize = bones.size();
			for(BoneIndex p = 0; p < count; ++p)
			{
				if( p != 0 )
				{
					errors += Check("bone index", p -1, boneSize);
				}
			}

			return errors;
		}

		template<typename T>
		void Copy(boost::scoped_array<T>& dst, const boost::scoped_array<T>& src, std::size_t count)
		{
			dst.reset(new T[count]);
			memcpy(dst.get(), src.get(), count);
		}

		void Mesh::doCopy(const Mesh& m)
		{
			count = m.count;

			Copy<real>(locations, m.locations, 3*count);
			Copy<real>(normals, m.normals, 3*count);
			Copy<real>(textcoords, m.textcoords, 2*count);
			Copy<BoneIndex>(boneindexes, m.boneindexes, count);

			bones = m.bones;
			triangles = m.triangles;
			materials = m.materials;
		}
	}
}
