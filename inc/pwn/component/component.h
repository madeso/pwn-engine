#ifndef PWN_COMPONENT_COMPONENT
#define PWN_COMPONENT_COMPONENT

#include <boost/noncopyable.hpp>
#include <map>
#include <boost/function.hpp>
#include <pwn/core/enum.h>
#include <pwn/number.h>

// for event declaration macros at the end
#include <functional>
#include <boost/bind.hpp>

namespace pwn
{
	namespace component
	{
		class EventArgs;

		// to implement
		class PropertyList
		{
		public:
			real& refReal();
		};

		/// Live part of a object.
		class Component
			: boost::noncopyable
		{
		protected:
			Component();
		public:
			virtual ~Component();

			/// Returns true if this should be removed.
			bool shouldBeRemoved() const;
			/// Marks the component for removal.
			void markForRemoval();

			void onEvent(const core::EnumValue& type, const EventArgs& args);

			void sendObjectEvent(const core::EnumValue& type, const EventArgs& args);

			// internal
			virtual void registerCallbacks() = 0;
			typedef boost::function<void (const EventArgs&)> Callback;
			void addCallback(const core::EnumValue& type, Callback c);
		protected:
			PropertyList locals; // local variables that automatically are saved to files
		private:
			bool removeSelf;
			typedef std::map<core::EnumValue, Callback> Map;
			Map callbacks;
		};
	}
}

#define BEGIN_EVENT_TABLE(X) void X::registerCallbacks() { typedef X C;
#define REGISTER_CALLBACK(e,f) addCallback(::pwn::component::EventArgs::Type().toEnum(#e),boost::bind(std::mem_fun(&C::f), this, _1))
#define END_EVENT_TABLE() }
#define DECLARE_CALLBACK() void registerCallbacks()

#endif
