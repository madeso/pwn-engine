#ifndef PWN_COMPONENT_COMPONENTSTD
#define PWN_COMPONENT_COMPONENTSTD

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace component
	{
		class Component;
		class ComponentDef;

		boost::shared_ptr<Component> CreateComponent(const ComponentDef& def);
	}
}

#endif
