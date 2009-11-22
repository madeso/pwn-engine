#include <pwn/core/idpool>

namespace pwn
{
	namespace core
	{
		IdPool::IdPool()
			: current(0)
		{
		}

		const IdPool::ID IdPool::generate()
		{
			if( released.empty())
			{
				const ID value = current;
				++current;
				return value;
			}
			else
			{
				const ID value = *released.rbegin();
				released.pop_back();
				return value;
			}
		}

		void IdPool::release(const IdPool::ID id)
		{
			released.push_back(id);
		}
	}
}