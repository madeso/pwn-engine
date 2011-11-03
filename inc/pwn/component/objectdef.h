#ifndef PWN_COMPONENT_OBJECTDEF
#define PWN_COMPONENT_OBJECTDEF

#include <boost/shared_ptr.hpp>
#include <pwn/string.h>
#include <map>
#include <vector>

namespace pwn
{
	namespace component
	{
		class Property;
		class ComponentDef;
		class Object;
		class ComponentCreator;

		class PropertyDef
		{
		public:
			enum Type
			{
				String, Real, Vec3, Quat
			};

			PropertyDef(Type t);
			Type getType() const;
		private:
			// add default value?
			Type type;
		};

		class ObjectDef
		{
		public:
			ObjectDef();
			~ObjectDef();

			void addProperty(const string& name, const PropertyDef& def);
			void addComponent(const ComponentDef& def);
			boost::shared_ptr<Object> create(const ComponentCreator& creator) const;
		private:
			typedef std::map<string, PropertyDef> Properties;
			Properties properties;
			typedef std::vector<ComponentDef> Components;
			Components components;
		};
	}
}

#endif
