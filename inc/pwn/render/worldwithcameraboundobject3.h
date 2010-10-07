#include <pwn/render/worldwith3.h>


namespace pwn
{
	namespace render
	{
		class WorldWithCameraBoundObject3 : public WorldWith3
		{
		public:
			WorldWithCameraBoundObject3(ActorPtr a, World3::Ptr w);
			virtual void render(int x, int y, int w, int h, const CompiledCamera& camera) const;
		private:
			ActorPtr actor;
		};
	}
}