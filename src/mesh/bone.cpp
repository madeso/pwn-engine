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

		const string Bone::toString()
		{
			if( parentBone == 0 ) return name;
			else return parentBone->toString() + " -> " + name;
		}

		bool Bone::hasParent() const
		{
			return parent != 0;
		}
	}
}
