#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include <pwn/core/stringutils.h>

namespace pwn
{
	namespace mesh
	{
		Bone::Bone()
			: rot( math::qIdentity() )
		{
			name[0] = 0;
		}

		bool Bone::hasParent() const
		{
			return parent != 0;
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
