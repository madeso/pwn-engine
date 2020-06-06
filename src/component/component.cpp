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

		void Component::onEvent(const core::EnumValue& type, const EventArgs& args)
		{
			Map::const_iterator r = callbacks.find(type);
			if(r != callbacks.end())
			{
				r->second(args);
			}
		}

		void Component::addCallback(const core::EnumValue& type, Callback c)
		{
			callbacks.insert(Map::value_type(type, c));
		}
	}
}
