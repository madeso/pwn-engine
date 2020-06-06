#ifndef PWN_COMPONENT_OBJECT
#define PWN_COMPONENT_OBJECT

#include <map>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pwn/string.h>

namespace pwn
{
	namespace core
	{
		class EnumValue;
	}

	namespace component
	{
		class Property;
		class Component;
		class EventArgs;
		class ObjectDef;

		class PropertyMap
			: boost::noncopyable
		{
		public:
			PropertyMap();
			~PropertyMap();

			void add(const string& name, boost::shared_ptr<Property> prop);
			boost::shared_ptr<Property> get(const string& name) const;
		private:
			typedef std::map<string, boost::shared_ptr<Property> > Map;
			Map map;
		};

		class ComponentList
			: boost::noncopyable
		{
		public:
			ComponentList();
			~ComponentList();

			void add(boost::shared_ptr<Component> component);
			void onEvent(const core::EnumValue& type, const EventArgs& args);
			void removePendingComponents();
		private:
			typedef std::vector<boost::shared_ptr<Component> > List;
			List list;
		};

		class Object
			: boost::noncopyable
		{
		public:
			Object();
			~Object();

			void addProperty(const string& name, boost::shared_ptr<Property> prop);
			void addComponent(boost::shared_ptr<Component> component);
			void onEvent(const core::EnumValue& type, const EventArgs& args);

		protected:
			friend class ObjectDef;
			const PropertyMap& getProperties() const;
		private:
			PropertyMap properties;
			ComponentList components;
		};
	}
}

#endif
