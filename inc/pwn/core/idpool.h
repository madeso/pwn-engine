#ifndef PWN_CORE_IDPOOL
#define PWN_CORE_IDPOOL

#include <vector>
#include "pwn/number.h"
#include "noncopyable.h"

namespace pwn
{
	namespace core
	{
		class IdPool : noncopyable
		{
		public:
			typedef uint32 ID;

			IdPool();

			const ID generate();
			void release(const ID id);
		private:
			ID current;
			std::vector<ID> released;
		};

		class Id
		{
		public:
			Id(IdPool* pool);
			~Id();

			const IdPool::ID value;
		private:
			IdPool* pool;
		};
	}
}

#endif
