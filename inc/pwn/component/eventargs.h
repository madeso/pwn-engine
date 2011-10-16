#ifndef PWN_COMPONENT_EVENTARGS
#define PWN_COMPONENT_EVENTARGS

#include <vector>
#include <boost/shared_ptr.hpp>
#include <pwn/core/enum.h>

#define pwnDeclareEventType(x) static const ::pwn::core::EnumValue x = ::pwn::component::EventArgs::Type().toEnum(#x)

namespace pwn
{
	namespace component
	{
		class Property;

		class EventArgs
		{
		public:
			EventArgs();
			~EventArgs();

			void add(boost::shared_ptr<Property> prop);
			EventArgs& operator<<(boost::shared_ptr<Property> prop);

			size_t count() const;

			static core::EnumType& Type();
		private:
			typedef std::vector<boost::shared_ptr<Property> > List;
			List list;
		};
	}
}

#endif
