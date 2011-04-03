#ifndef PWN_CORE_VEC_H
#define PWN_CORE_VEC_H

#include <vector>

namespace pwn
{
	namespace core
	{
		template<typename T>
		class Vec
		{
		public:
			Vec<T>& operator<<(const T& t)
			{
				vec.push_back(t);
				return *this;
			}

			operator std::vector<T>() const
			{
				return vec;
			}

			std::vector<T> vec;
		};
	}
}

#endif
