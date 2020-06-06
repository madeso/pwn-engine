#include <pwn/component/objectdef.h>
#include <pwn/component/componentdef.h>
#include <pwn/component/object.h>
#include <boost/foreach.hpp>
#include <pwn/component/propertystd.h>
#include <pwn/math/operations.h>
#include <pwn/component/componentstd.h>

namespace pwn
{
	namespace component
	{
		PropertyDef::PropertyDef(PropertyDef::Type t)
			: type(t)
		{
		}

		PropertyDef::Type PropertyDef::getType() const
		{
			return type;
		}

		ObjectDef::ObjectDef()
		{
		}

		ObjectDef::~ObjectDef()
		{
		}

		void ObjectDef::addProperty(const string& name, const PropertyDef& def)
		{
			properties.insert(Properties::value_type(name, def));
		}

		void ObjectDef::addComponent(const ComponentDef& def)
		{
			components.push_back(def);
		}

		namespace
		{
			boost::shared_ptr<Property> CreateProperty(const PropertyDef& def)
			{
				switch(def.getType())
				{
				case PropertyDef::String:
					return property::CreateString("");
				case PropertyDef::Real:
					return property::CreateReal(0);
				case PropertyDef::Vec3:
					return property::CreateVec3(math::vec3(0,0,0));
				case PropertyDef::Quat:
					return property::CreateQuat(math::qIdentity());
				default:
					throw "unknown property type";
				//boost::shared_ptr<Property> CreateEvent(const core::EnumValue& e);
				}
			}
		}

		boost::shared_ptr<Object> ObjectDef::create(const ComponentCreator& creator) const
		{
			boost::shared_ptr<Object> r( new Object() );

			BOOST_FOREACH(const Properties::value_type& v, properties)
			{
				r->addProperty(v.first, CreateProperty(v.second));
			}

			BOOST_FOREACH(const ComponentDef& def, components)
			{
				r->addComponent(creator.create(def.name, r->getProperties(), def.args));
			}

			return r;
		}
	}
}
