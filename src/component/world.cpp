#include <pwn/component/world.h>
#include <pwn/component/eventargs.h>
#include <boost/foreach.hpp>
#include <pwn/component/object.h>
#include <pwn/component/propertystd.h>

namespace events
{
	pwnDeclareEventType(Update);
	pwnDeclareEventType(Render);
}

namespace pwn
{
	namespace component
	{
		World::World()
		{
		}
		World::~World()
		{
		}

		void World::add(ObjectPtr o)
		{
			objects.push_back(o);
		}

		void World::update(real delta)
		{
			sendEvent(events::Update, EventArgs()<<property::CreateReal(delta));
		}

		void World::render()
		{
			sendEvent(events::Render, EventArgs());
		}

		void World::sendEvent(const core::EnumValue& type, const EventArgs& args)
		{
			BOOST_FOREACH(ObjectPtr& o, objects)
			{
				o->onEvent(type, args);
			}
		}
	}
}
