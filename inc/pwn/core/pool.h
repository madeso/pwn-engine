#ifndef PWN_CORE_POOL
#define PWN_CORE_POOL

#include <boost/function.hpp>
#include <vector>

namespace pwn
{
	namespace core
	{
		template<typename T>
		class Pool
		{
		public:
			typedef boost::function<T(void)> CreateFunction;

			Pool(CreateFunction acreator)
				: creator(acreator)
			{
			}

			T get()
			{
				if (pool.empty()) return creator();
				else
				{
					T temp = pool.back();
					pool.pop_back();
					return temp;
				}
			}

			void release(T t)
			{
				pool.push_back(t);
			}
		private:
			CreateFunction creator;
			std::vector<T> pool;
		};
	}
}

#endif
