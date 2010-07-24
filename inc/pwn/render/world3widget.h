#ifndef PWN_RENDER_WORLD3_WIDGET
#define PWN_RENDER_WORLD3_WIDGET

#include <pwn/render/widget.h>
#include <pwn/render/camera.h>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class World3;

		class World3Widget : public Widget
		{
		public:
			World3Widget(const Dock& dock, boost::shared_ptr<World3> world);
			~World3Widget();

			void setCamera(const Camera& camera);

			void render(int width, int height);
		private:
			boost::shared_ptr<World3> world;
			Camera camera;
		};
	}
}

#endif