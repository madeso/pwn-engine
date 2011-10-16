#ifndef PWN_COMPONENT_WORLD
#define PWN_COMPONENT_WORLD

#include <boost/shared_ptr.hpp>
#include <pwn/math/types.h>
#include <vector>

namespace pwn
{
	namespace core
	{
		class EnumValue;
	}
	namespace component
	{
		class Object;
		class EventArgs;

		class World
		{
		public:
			typedef boost::shared_ptr<Object> ObjectPtr;
			World();
			~World();

			void add(ObjectPtr o);
			void update(real delta);
			void render();

			void sendEvent(const core::EnumValue& type, const EventArgs& args);
		private:
			// implement better structure
			std::vector<ObjectPtr> objects;
		};
	}
}

#endif
