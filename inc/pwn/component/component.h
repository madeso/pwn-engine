#ifndef PWN_COMPONENT_COMPONENT
#define PWN_COMPONENT_COMPONENT

#include <boost/noncopyable.hpp>

namespace pwn
{
	namespace component
	{
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
			virtual void onEvent() = 0;

			/// Marks the component for removal.
			void markForRemoval();
		private:
			bool removeSelf;
			PropertyList locals; // local variables that automatically are saved to files
		};
	}
}

#endif
