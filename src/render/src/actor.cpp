#include <pwn/render/actor.h>
#include <pwn/render/renderlist.h>
#include <pwn/render/actordef.h>
#include <pwn/render/part.h>
#include <pwn/math/operations.h>
#include <pwn/render/compiledcamera.h>

#pragma warning (disable:4512) // boost\utility\addressof.hpp(30) : warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
	namespace render
	{
		Actor::Actor(const math::point3& location, const math::quat& rotation, boost::shared_ptr<ActorDef> model)
			: location(location)
			, rotation(rotation)
			, model(model)
		{
		}

		boost::shared_ptr<Actor> Actor::Create(const math::point3& location, const math::quat& rotation, boost::shared_ptr<ActorDef> model)
		{
			boost::shared_ptr<Actor> a( new Actor(location, rotation, model) );
			return a;
		}

		void Actor::render(RenderList* rl, const CompiledCamera& cam)
		{
			if( model == 0 ) return; // abort if no model available

			const math::mat44 mat = cam.mat * cmat44(location.vec) * cmat44(rotation);

			BOOST_FOREACH(ActorDef::PartPtr p, model->parts)
			{
				rl->add(p->mesh, p->material, mat, this);
			}
		}

		void Actor::setPose(const mesh::Pose& p)
		{
			pose = mesh::CompiledPose(p, model->bones);
		}
	}
}
