// pwn/stdutil

#ifndef PWN_STDUTIL
#define PWN_STDUTIL

#include <algorithm>
#include <vector>
#include <pwn/string.h>

namespace pwn
{
	namespace core
	{
		// todo: provide an indexed version of this function as well.
		template<typename pwnType>
		void SwapBackAndEraseObject(pwnType what, std::vector<pwnType>* from)
		{
			std::vector<int>::iterator iii;
			typedef std::vector<pwnType> Vec;
			typename Vec::iterator result = std::find(from->begin(), from->end(), what);
			if(result == from->end())
			{
				return;
			}
			typename Vec::iterator last = from->end();
			last--; // point to a valid entry
			std::swap(*result, *last);
			from->pop_back();
		}

		template<typename T>
		class Vec
		{
		public:
			Vec& operator<<(const T& t)
			{
				ts.push_back(t);
				return *this;
			}

			operator std::vector<T>()
			{
				return ts;
			}

			std::vector<T> ts;
		};
	}
}

#endif
