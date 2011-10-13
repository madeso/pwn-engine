#include <pwn/component/component.h>

namespace pwn
{
	namespace component
	{
		Component::Component()
			: removeSelf(false)
		{
			// empty
		}

		Component::~Component()
		{
			// empty
		}

		bool Component::shouldBeRemoved() const
		{
			return removeSelf;
		}

		void Component::markForRemoval()
		{
			removeSelf = true;
		}
	}
}
