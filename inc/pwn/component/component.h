#ifndef PWN_COMPONENT_COMPONENT
#define PWN_COMPONENT_COMPONENT

#include <boost/noncopyable.hpp>
#include <map>
#include <boost/function.hpp>
#include <pwn/core/enum.h>

namespace pwn
{
	namespace component
	{
		class EventArgs;

		// to implement
		class PropertyList
		{
		};

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

			// internal
			virtual void registerCallbacks() = 0;
			typedef boost::function<void (const EventArgs&)> Callback;
			void addCallback(const core::EnumValue& type, Callback c);
		private:
			bool removeSelf;
			PropertyList locals; // local variables that automatically are saved to files
			typedef std::map<core::EnumValue, Callback> Map;
			Map callbacks;
		};
	}
}

#endif
