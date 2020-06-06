#ifndef PWN_COMPONENT_PROPERTYSTD
#define PWN_COMPONENT_PROPERTYSTD

#include <boost/shared_ptr.hpp>
#include <pwn/component/property.h>
namespace pwn
{
	namespace component
	{
		namespace property
		{
			boost::shared_ptr<Property> CreateString(const string& s);
			boost::shared_ptr<Property> CreateReal(real r);
			boost::shared_ptr<Property> CreateVec3(const math::vec3& v);
			boost::shared_ptr<Property> CreateQuat(const math::quat& q);
			boost::shared_ptr<Property> CreateEvent(const core::EnumValue& e);
		}
	}
}

#endif
