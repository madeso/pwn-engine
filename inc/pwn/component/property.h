#ifndef PWN_COMPONENT_PROPERTY
#define PWN_COMPONENT_PROPERTY

#include <boost/noncopyable.hpp>
#include <pwn/string.h>
#include <pwn/math/types.h>

namespace pwn
{
	namespace core
	{
		class EnumValue;
	}

	namespace component
	{
		class Property
			: boost::noncopyable
		{
		protected:
			Property();
		public:
			virtual ~Property();

			// functions for saving and loading from/to files
			// functions for saving and loading from/to script variables

			virtual const string getString() const = 0;
			virtual const real getReal() const = 0;
			virtual const math::vec3 getVec3() const = 0;
			virtual const math::quat getQuat() const = 0;
			virtual const core::EnumValue getEvent() const = 0;

			virtual string& refString() = 0;
			virtual real& refReal() = 0;
			virtual math::vec3& refVec3() = 0;
			virtual math::quat& refQuat() = 0;

			virtual void setString(const string& s) = 0;
			virtual void setReal(real r) = 0;
			virtual void setVec3(const math::vec3& v) = 0;
			virtual void setQuat(const math::quat& q) = 0;
			virtual void setEvent(const core::EnumValue& e) = 0;
		private:
		};
	}
}

#endif
