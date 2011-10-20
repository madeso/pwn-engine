#ifndef PWN_COMPONENT_COMPONENTSTD
#define PWN_COMPONENT_COMPONENTSTD

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <map>
#include <pwn/string.h>

namespace pwn
{
	namespace component
	{
		class Component;
		class PropertyMap;
		class ComponentArgs;

		class ComponentCreator
		{
		public:
			typedef string ID; // change to EnumValue?
			typedef boost::function<boost::shared_ptr<Component> (const PropertyMap& props, const ComponentArgs& args)> CreateFunction;

			ComponentCreator();
			~ComponentCreator();
			boost::shared_ptr<Component> create(const ID& name, const PropertyMap& props, const ComponentArgs& args) const;
			void add(const ID& name, CreateFunction cf);
		private:
			typedef std::map<ID, CreateFunction> Functions;
			Functions functions;
		};

		void AddStandardComponents(ComponentCreator* cc);
	}
}

#endif
