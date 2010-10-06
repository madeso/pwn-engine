#ifndef PWN_RENDER_WORLDWITH3
#define PWN_RENDER_WORLDWITH3

#include <pwn/render/world3.h>

namespace pwn
{
	namespace render
	{
		class WorldWith3 : public World3
		{
		public:
			WorldWith3(World3::Ptr w);
			virtual ~WorldWith3();
			
			virtual void actor_add(ActorPtr actor);
			virtual void actor_remove(ActorPtr actor);

			virtual void render(int x, int y, int w, int h, const CompiledCamera& camera);
		private:
			World3::Ptr world;
		};
	}
}

#endif