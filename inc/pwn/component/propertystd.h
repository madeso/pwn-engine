#ifndef PWN_COMPONENT_PROPERTYSTD
#define PWN_COMPONENT_PROPERTYSTD

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace component
	{
		class Property;
		class PropertyDef;

		boost::shared_ptr<Property> CreateProperty(const PropertyDef& def);
	}
}

#endif
