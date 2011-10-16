#ifndef PWN_COMPONENT_COMPONENTDEF
#define PWN_COMPONENT_COMPONENTDEF

#include <map>
#include <pwn/string.h>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace component
	{
		class Property;

		class ComponentArgs
		{
		public:
			ComponentArgs();
			~ComponentArgs();

			typedef boost::shared_ptr<Property> PropertyPtr;
			void add(const string& s, PropertyPtr prop);
			PropertyPtr get(const string& s) const;

			template<class T>
			ComponentArgs& operator()(const string& s, const T& t)
			{
				add(s, t);
				return *this;
			}
		private:
			typedef std::map<string, PropertyPtr> Vars;
			Vars vars;
		};

		class ComponentDef
		{
		public:
			// EnumValue id;
			ComponentArgs args;
		private:
		};
	}
}

#endif
