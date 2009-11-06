#include <pwn/render/actor>
#include <pwn/render/renderlist>
#include <pwn/render/actordef>

#pragma warning (disable:4512) // boost\utility\addressof.hpp(30) : warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
	namespace render
	{
		Actor::Actor(const math::point3& location, const math::quat& rotation)
			: location(location)
			, rotation(rotation)
		{
		}

		void Actor::render(RenderList* rl, const Camera&)
		{
			if( model == 0 ) return; // abort if no model available

			BOOST_FOREACH(ActorDef::PartPtr p, model->parts)
			{
				// todo: use camera and add a matrix instead
				rl->add(p, location, rotation);
			}
		}
	}
}
