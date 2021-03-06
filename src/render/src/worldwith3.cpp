#include <pwn/render/worldwith3.h>


namespace pwn
{
namespace render
{
    WorldWith3::WorldWith3(World3::Ptr w) : world(w)
    {
    }

    WorldWith3::~WorldWith3()
    {
    }

    void
    WorldWith3::actor_add(ActorPtr actor)
    {
        world->actor_add(actor);
    }

    void
    WorldWith3::actor_remove(ActorPtr actor)
    {
        world->actor_remove(actor);
    }

    void
    WorldWith3::lines_add(LinesPtr lines)
    {
        world->lines_add(lines);
    }
    void
    WorldWith3::lines_remove(LinesPtr lines)
    {
        world->lines_remove(lines);
    }

    void
    WorldWith3::light_add(LightPtr light)
    {
        world->light_add(light);
    }

    void
    WorldWith3::light_remove(LightPtr light)
    {
        world->light_remove(light);
    }

    void
    WorldWith3::light_setAmbient(const math::Rgba& ambient)
    {
        world->light_setAmbient(ambient);
    }

    void
    WorldWith3::render(const RenderArgs& r) const
    {
        world->render(r);
    }
}
}
