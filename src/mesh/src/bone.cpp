#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include <pwn/core/stringutils.h>
#include <pwn/assert.h>

namespace pwn
{
	namespace mesh
	{
		Bone::Bone()
			: rot( math::qIdentity() )
			, parent(0)
		{
			name[0] = 0;
		}

		bool Bone::hasParent() const
		{
			return parent != 0;
		}

		BoneIndex Bone::getParent() const
		{
			Assert(parent != 0);
			return parent -1;
		}

		void Bone::setParent(BoneIndex bi)
		{
			parent = bi +1;
		}

		void Bone::setName(const string& n)
		{
			core::Copy(name, n, BoneNameSize::Value);
		}

		string Bone::getName() const
		{
			return name;
		}
	}
}
