#ifndef PWN_ENGINE_DEMOCONTROLS
#define PWN_ENGINE_DEMOCONTROLS

#include <pwn/engine/key.h>
#include <pwn/math/types.h>

namespace pwn
{
	namespace engine
	{
		class DemoControls
		{
		public:
			DemoControls();
			~DemoControls();

			Key::Code forward;
			Key::Code backward;
			Key::Code left;
			Key::Code right;
			Key::Code up;
			Key::Code down;

			bool onKey(Key::Code key, bool newState);
			void onMouse(const math::vec2 movement);
			void update(math::point3* position, math::quat* rotation, const real delta, const real speed, const real sensitivity);
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
