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

		Triangle::VertexIndex Mesh::add(const math::vec3& pos, const math::vec2& text, const math::vec3& normal, const BoneIndex bone)
		{
			const Triangle::VertexIndex r = positions.size();
			positions.push_back( Point(pos, normal, text, bone) );
			return r;
		}

		void Mesh::clear()
		{
			positions.clear();
			triangles.clear();
			bones.clear();
			materials.clear();
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
			const std::size_t positionSize = positions.size();
			const std::size_t materialSize = materials.size();
			BOOST_FOREACH(const TriangleMap::value_type& mt, triangles)
			{
				errors += Check("material index", mt.first, materialSize);
				BOOST_FOREACH(const pwn::mesh::Triangle& t, mt.second)
				{
					for(int i=0; i<3; ++i)
					{
						errors += Check("position index", t[i], positionSize);
					}
				}
			}

			// test that the points are valid
			const std::size_t boneSize = bones.size();
			BOOST_FOREACH(const Point& p, positions)
			{
				if( p.hasBone() )
				{
					errors += Check("bone index", p.getBone(), boneSize);
				}
			}

			return errors;
		}

		class BoneToSort
		{
		public:
			BoneIndex index;
			BoneToSort* parent;
			std::vector<BoneToSort*> children;

			void traverse(std::vector<BoneIndex>* list) const
			{
				list->push_back(index);
				BOOST_FOREACH(BoneToSort* b, children)
				{
					b->traverse(list);
				}
			}
		};


		void Flatouter::load(const Mesh& mesh)
		{
			newIndices.clear();/*

			std::vector<BoneToSort> bones;
			for(BoneIndex i=0; i<mesh.bones.size(); ++i)
			{
				BoneToSort b;
				b.index = i;
				b.parent = 0;
				bones.push_back(b);
			}
			for(BoneIndex i=0; i<bones.size(); ++i)
			{
				if( mesh.bones[i].hasParent() )
				{
					bones[i].parent = &bones[mesh.bones[i].getParent()];
				}
			}
			for(BoneIndex i=0; i<bones.size(); ++i)
			{
				if( bones[i].parent )
				{
					bones[i].parent->children.push_back(&bones[i]);
				}
			}
			for(BoneIndex i=0; i<mesh.bones.size(); ++i)
			{
				if( bones[i].parent == 0 )
				{
					bones[i].traverse(&newIndices);
				}
			}*/
		}

		void Flatouter::modify(Mesh* mesh) const
		{
			if( newIndices.empty() ) return;
			BOOST_FOREACH(Point& p, mesh->positions)
			{
				p.bone = newIndices[p.bone];
			}

			std::vector<Bone> bs = mesh->bones;
			for(pwn::uint32 i=0; i<mesh->bones.size(); ++i)
			{
				mesh->bones[i] = bs[newIndices[i]];
				Bone& b = mesh->bones[i];
				if( b.hasParent() )
				{
					b.setParent(newIndices[b.getParent()]);
				}
			}
		}

		void Flatouter::modify(Animation* animation) const
		{
			if( newIndices.empty() ) return;

			std::vector<AnimationPerBone> apb = animation->bones;
			for(pwn::uint32 i=0; i<animation->bones.size(); ++i)
			{
				animation->bones[i] = apb[newIndices[i]];
			}
		}
	}
}
