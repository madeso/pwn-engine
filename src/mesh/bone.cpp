#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace mesh
	{
		Bone::Bone()
			: rot( math::qIdentity() )
		{
		}

		bool Bone::hasParent() const
		{
			return parent != 0;
		}
	}
}
