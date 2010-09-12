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

		void Mesh::clear()
		{
			positions.clear();
			normals.clear();
			texcoords.clear();
			triangles.clear();
			bones.clear();
			materials.clear();
		}

		pwn::uint32 Mesh::validate() const
		{
			pwn::uint32 errors = 0;
			for(std::size_t i = 0; i<bones.size(); ++i)
			{
				if( bones[i].hasParent() && bones[i].getParent() >= i ) ++errors; // bones not sorted
			}

			/*
			uint32 errors = 0;

			const std::size_t positions = mBuilder.positions.size();
			const std::size_t normals = mBuilder.normals.size();
			const std::size_t texcoords = mBuilder.texcoords.size();
			const std::size_t materials = mBuilder.materials.size();

			for(pwn::mesh::Triangle::index i=0; i<mBuilder.triangles.size(); ++i)
			{
				const pwn::mesh::Triangle t = mBuilder.triangles[i];
				errors += Check("material index", t.material, materials);
				for(int i=0; i<3; ++i)
				{
					errors += Check("position index", t[i].location, positions);
					if( normals != 0 )
					{
						errors += Check("normal index", t[i].normal, normals);
					}
					if( texcoords != 0 )
					{
						errors += Check("texcoord index", t[i].texcoord, texcoords);
					}
				}
			}
			*/

			return errors; // todo: change to true when all data have been checked
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
