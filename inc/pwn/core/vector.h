#ifndef PWN_CORE_VECTOR
#define PWN_CORE_VECTOR

#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

namespace pwn
{
	namespace core
	{
		template <typename T>
		class Vector
			: boost::noncopyable
		{
		public:
			typedef std::size_t SizeType;

			void reset(SizeType s)
			{
				arr.reset( new T[s] );
				length = s;
			}

			SizeType size() const
			{
				return length;
			}
		private:
			boost::scoped_array<T> arr;
			SizeType length;
		};
	}
}

#endif