#include <pwn/render/worldwith3.h>


namespace pwn
{
	namespace render
	{
		/** A world with a single camera bound object, like a sky-sphere.
		*/
		class WorldWithCameraBoundObject3 : public WorldWith3
		{
		public:
			WorldWithCameraBoundObject3(ActorPtr a, World3::Ptr w);
			virtual void render(const RenderArgs& ra) const;
		private:
			ActorPtr actor;
		};
	}
}
