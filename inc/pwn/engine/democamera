#ifndef PWN_ENGINE_DEMOCAMERA
#define PWN_ENGINE_DEMOCAMERA

#include <pwn/render/camera>
#include <pwn/engine/Key>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class World3Widget;
	}

	namespace engine
	{
		class DemoCamera
		{
		public:
			DemoCamera();
			~DemoCamera();

			render::Camera camera;

			Key::Code forward;
			Key::Code backward;
			Key::Code left;
			Key::Code right;
			Key::Code up;
			Key::Code down;

			bool onKey(Key::Code key, bool newState);
			void onMouse(const math::vec2 movement);
			void update(const real delta, const real speed, const real sensitivity);

			boost::shared_ptr<render::World3Widget> world;
		private:
			bool forwardState;
			bool backwardState;
			bool leftState;
			bool rightState;
			bool upState;
			bool downState;

			math::vec2 mouse;
		};
	}
}

#endif