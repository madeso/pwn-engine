#ifndef PWN_CORE_VECTOR
#define PWN_CORE_VECTOR

#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range.hpp>
#include <vector>

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
			typedef std::ptrdiff_t Index;

			explicit Vector(SizeType s)
				: length(0)
			{
				reset(s);
			}

			T* get()
			{
				return arr.get();
			}

			const T* get() const
			{
				return arr.get();
			}

			explicit Vector(Vector<T>& other)
				: length(0)
			{
				swap(other);
			}

			Vector()
				: length(0)
			{
			}
			
			explicit Vector(const std::vector<T>& v)
			{
				*this = v;
			}

			explicit Vector(bool, const Vector<T>& v)
			{
				reset(v.size());
				for(SizeType i=0; i<length; ++i)
				{
					arr[i]  = v[i];
				}
			}

			void reset(SizeType s)
			{
				arr.reset( new T[s] );
				length = s;
			}

			void clear()
			{
				arr.reset();
				length = 0;
			}

			SizeType size() const
			{
				return length;
			}

			void operator=(const std::vector<T>& v)
			{
				reset(v.size());
				for(SizeType i=0; i<length; ++i)
				{
					arr[i]  = v[i];
				}
			}

			void swap(Vector<T>& other)
			{
				arr.swap(other.arr);
				//std::swap(arr, other.arr);
				std::swap(length, other.length);
			}

			T& at(Index i)
			{
				return arr[i];
			}

			T& operator[](Index i)
			{
				return arr[i];
			}

			const T& at(Index i) const
			{
				return arr[i];
			}

			const T& operator[](Index i) const
			{
				return arr[i];
			}

			template<typename It>
			class iter
				: public boost::iterator_facade<iter<It>, It, boost::forward_traversal_tag> // todo: use a bidirectional tag
			{
			public:
				typedef Vector<It> Vec;

				iter()
				{ }

				explicit iter(std::ptrdiff_t i, Vec* v)
					: index(i)
					, vec(v)
				{
				}

			public:
				bool equal(iter<It> const& other) const
				{
					return index == other.index;
				}

				void increment()
				{
					++index;
				}

				It& dereference() const
				{
					return vec->at(index);
				}
			private:
				std::ptrdiff_t index;
				Vec* vec;
			};

			typedef iter<T> iterator;
			typedef iter<T const> const_iterator;

			iterator begin()
			{
				return iterator(0, this);
			}

			iterator end()
			{
				return iterator(length + 1, this);
			}

			const_iterator begin() const
			{
				return const_iterator(0, this);
			}

			const_iterator end() const
			{
				return const_iterator(length + 1, this);
			}

		private:
			boost::scoped_array<T> arr;
			SizeType length;
		};
	}
}

namespace boost
{
	template< typename T >
	struct range_mutable_iterator< pwn::core::Vector<T> >
	{
		typedef typename pwn::core::Vector<T>::iterator type;
	};

	template< typename T >
	struct range_const_iterator< pwn::core::Vector<T> >
	{
		typedef typename pwn::core::Vector<T>::const_iterator type;
	};

}

namespace pwn
{
	namespace core
	{
		template< class T >
		inline typename T::iterator range_begin( Vector<T>& x )
		{ 
			return x.begin();
		}

		template< class T >
		inline typename T::const_iterator range_begin( const Vector<T>& x )
		{ 
			return x.begin();
		}

		template< class T >
		inline typename T::iterator range_end( Vector<T>& x )
		{ 
			return x.end();
		}

		template< class T >
		inline typename T::const_iterator range_end( const Vector<T>& x )
		{ 
			return x.end();
		}

	}
}

#endif