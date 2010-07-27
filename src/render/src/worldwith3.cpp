#include <pwn/render/worldwith3.h>


namespace pwn
{
	namespace render
	{
		WorldWith3::WorldWith3(World3::Ptr w)
			: world(w)
		{
		}

		WorldWith3::~WorldWith3()
		{
		}
		
		void WorldWith3::actor_add(ActorPtr actor)
		{
			world->actor_add(actor);
		}

		void WorldWith3::actor_remove(ActorPtr actor)
		{
			world->actor_remove(actor);
		}

		void WorldWith3::render(const Camera& camera)
		{
			world->render(camera);
		}
	}
}