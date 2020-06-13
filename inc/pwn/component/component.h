#ifndef PWN_COMPONENT_COMPONENT
#define PWN_COMPONENT_COMPONENT

#include "noncopyable.h"

#include <functional>
#include <map>

#include "pwn/core/enum.h"
#include "pwn/number.h"

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
			: noncopyable
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
			typedef std::function<void (const EventArgs&)> Callback;
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

// todo(Gustav): fix
#define BEGIN_EVENT_TABLE(X) void X::registerCallbacks() { typedef X C;
#define REGISTER_CALLBACK(e,f) addCallback(::pwn::component::EventArgs::Type().toEnum(#e),boost::bind(std::mem_fun(&C::f), this, _1))
#define END_EVENT_TABLE() }
#define DECLARE_CALLBACK() void registerCallbacks()

#endif
