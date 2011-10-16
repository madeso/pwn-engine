#ifndef PWN_CORE_ENUM
#define PWN_CORE_ENUM

#include <pwn/string.h>
#include <map>
#include <vector>

namespace pwn
{
	namespace core
	{
		class EnumValue;

		/// Represents a enum type. Declare globally grab specific enums before or after load, and load adds new, verifies created and asserts misspelled values.
		class EnumType
		{
		public:
			EnumType();
			~EnumType();

			const string& toString(size_t v) const;
			const EnumValue toEnum(const string& name);
			void addEnum(const string& name);
			void stopAdding();
		private:
			typedef std::map<size_t, string> List;
			typedef std::map<string,size_t> Map;
			List list;
			Map map;

			List createdButNotAddedList;
			Map createdButNotAddedMap;
			bool isAdding;
			size_t nextIndex;
		};

		class EnumValue
		{
		public:
			EnumValue(EnumType* type, size_t value);

			const string toString() const;
			const size_t toValue() const;
			bool operator==(const EnumValue& other) const;
			bool operator!=(const EnumValue& other) const;
			bool operator<(const EnumValue& other) const;
		private:
			EnumType* type;
			size_t value;
		};

		std::ostream& operator<<(std::ostream& s, const EnumValue& v);
	}
}

#endif
