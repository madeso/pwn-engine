#ifndef PWN_CORE_IDPOOL
#define PWN_CORE_IDPOOL

#include <pwn/number.h>
#include <vector>
#include <boost/noncopyable.hpp>

namespace pwn
{
	namespace core
	{
		class IdPool : boost::noncopyable
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

		class Id : boost::noncopyable
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
