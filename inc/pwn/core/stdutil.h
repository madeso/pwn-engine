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
		template<typename Type>
		void SwapBackAndEraseObject(Type what, std::vector<Type>* from)
		{
			const std::vector<Type>::iterator result = std::find(from->begin(), from->end(), what);
			if( result == from->end() ) return;
			std::vector<Type>::iterator last = from->end();
			last--; // point to a valid entry
			std::swap(*result, *last);
			from->pop_back();
		}
	}
}

#endif