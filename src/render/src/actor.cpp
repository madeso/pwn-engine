#include <pwn/render/actor.h>
#include <pwn/render/renderlist.h>
#include <pwn/render/actordef.h>
#include <pwn/render/part.h>
#include <pwn/math/operations.h>
#include <pwn/render/compiledcamera.h>

#pragma warning(                                                               \
        disable : 4512)  // boost\utility\addressof.hpp(30) : warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
namespace render
{
    Actor::Actor(
            const math::point3& position,
            const math::quat& rotation,
            boost::shared_ptr<ActorDef> model)
        : position(position)
        , rotation(rotation)
        , model(model)
        , debug(false)
    {
    }

    boost::shared_ptr<Actor>
    Actor::Create(
            const math::point3& position,
            const math::quat& rotation,
            boost::shared_ptr<ActorDef> model)
    {
        boost::shared_ptr<Actor> a(new Actor(position, rotation, model));
        return a;
    }

    void
    Actor::render(RenderList* rl, const CompiledCamera& cam)
    {
        const math::mat44 mat = cam.generateMatrix(position.vec, rotation);

        if (debug)
        {
            rl->add(mat);
        }

        if (model == 0)
        {
            return;  // abort if no model available
        }

        BOOST_FOREACH (ActorDef::PartPtr p, model->parts)
        {
            rl->add(p->mesh, p->material, mat, this);
        }
        rl->add(mat, this);
    }

    void
    Actor::setPose(const mesh::Pose& p)
    {
        pose = mesh::CompiledPose(p, model->bones);
    }
}
}
