#ifndef PWN_CORE_STRINGSEPERATOR
#define PWN_CORE_STRINGSEPERATOR

#include <pwn/string.h>
#include <pwn/core/str.h>

namespace pwn
{
	namespace core
	{
		class StringSeperator
		{
		public:
			typedef std::vector<string> Vec;

			string toString() const
			{
				if(strings.empty())
				{
					return Str() << start << mempty << end;
				}

				ostringstream ss;

				ss << start;

				const Vec::size_type count = strings.size();
				for(Vec::size_type index = 0; index < count; ++index)
				{
					ss << strings[index];

					if(count != index + 1)  // if this item isnt the last one in the list
					{
						if(count == index + 2)
						{
							ss << fisep;
						}
						else
						{
							ss << sep;
						}
					}
				}

				ss << end;

				return ss.str();
			}

			StringSeperator& english()
			{
				return between(", ", " and ")
				       .startend("", "");
			}

			StringSeperator& array()
			{
				return between(", ")
				       .startend("[", "]");
			}

			StringSeperator& between(const string& seperator, const string finalSeperator)
			{
				sep = seperator;
				fisep = finalSeperator;
				return *this;
			}

			StringSeperator& between(const string& seperator)
			{
				sep = seperator;
				fisep = seperator;
				return *this;
			}

			StringSeperator& empty(const string& empty)
			{
				mempty = empty;
				return *this;
			}

			StringSeperator& startend(const string& astart, const string& aend)
			{
				start = astart;
				end = aend;
				return *this;
			}

			// move to a external util function and move whole class implementation to a cpp file?
			template<class C>
			StringSeperator& iterate(const C& c)
			{
				for(typename C::const_iterator i = c.begin(); i != c.end(); ++i)
				{
					strings.push_back(Str() << *i);
				}
				return *this;
			}
		private:
			Vec strings;

			string sep;
			string fisep;
			string mempty;
			string start;
			string end;
		};
	}
}

#endif
