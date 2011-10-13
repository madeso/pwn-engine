#include <pwn/component/object.h>

#include <pwn/component/property.h>
#include <boost/foreach.hpp>
#include <pwn/component/component.h>


namespace pwn
{
	namespace component
	{
		///////////////////////////////////////////////////////////////////////////////////
		// PopertyMap

		PropertyMap::PropertyMap()
		{
			// empty
		}

		PropertyMap::~PropertyMap()
		{
			// empty
		}

		void PropertyMap::add(const string& name, boost::shared_ptr<Property> prop)
		{
			map.insert(Map::value_type(name, prop));
		}

		boost::shared_ptr<Property> PropertyMap::get(const string& name) const
		{
			Map::const_iterator r = map.find(name);
			if( r == map.end() )
			{
				throw "property not added";
			}

			return r->second;
		}

		///////////////////////////////////////////////////////////////////////////////////
		// ComponentList

		ComponentList::ComponentList()
		{
			// empty
		}

		ComponentList::~ComponentList()
		{
			// empty
		}

		void ComponentList::add(boost::shared_ptr<Component> component)
		{
			list.push_back(component);
		}

		void ComponentList::onEvent()
		{
			BOOST_FOREACH(boost::shared_ptr<Component> c, list)
			{
				if( c->shouldBeRemoved() == false )
				{
					c->onEvent();
				}
			}
		}

		namespace
		{
			bool ShouldBeRemoved(boost::shared_ptr<Component> c)
			{
				return c->shouldBeRemoved();
			}
		}

		void ComponentList::removePendingComponents()
		{
			list.erase(std::remove_if(list.begin(), list.end(), ShouldBeRemoved), list.end());
		}

		///////////////////////////////////////////////////////////////////////////////////
		// Object

		Object::Object()
		{
		}

		Object::~Object()
		{
		}
	}
}
