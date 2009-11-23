#include <pwn/engine/mousefilter>
#include <pwn/math/operations>

#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
	namespace engine
	{
		// todo: implement a time-based instead of a frame-based version
		MouseFilter::MouseFilter()
			: factor(0.0f)
			, buffersize(10)
		{
		}

		void MouseFilter::setBufferSize(const std::size_t size)
		{
			buffersize = size;
			// todo: fix this ugly hack
			if( buffer.size() >= buffersize ) buffer.pop_back();
		}

		void MouseFilter::update(const math::vec2& movement)
		{
			if( buffer.size() >= buffersize ) buffer.pop_back();
			buffer.push_front(movement);
		}

		const math::vec2 MouseFilter::getMovement() const
		{
			real f = 1;
			math::vec2 r(0,0);
			std::size_t count = 0;
			real d = 0;
			BOOST_FOREACH(const math::vec2& m, buffer)
			{
				r += m * f;
				++ count;
				d += f;
				f *= factor;
				if( f <= 0 ) break;
			}
			if( count > 0 )
			{
				return r / d;
			}
			else
			{
				return r;
			}
		}
	}
}