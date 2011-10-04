#ifndef PWN_ENGINE_DEMOMOVEMENT
#define PWN_ENGINE_DEMOMOVEMENT

#include <pwn/engine/key.h>
#include <pwn/math/types.h>

namespace pwn
{
	namespace engine
	{
		class DemoMovement
		{
		public:
			DemoMovement();
			~DemoMovement();

			Key::Code forward;
			Key::Code backward;
			Key::Code left;
			Key::Code right;
			Key::Code up;
			Key::Code down;

			bool localUp;

			bool onKey(Key::Code key, bool newState);
			void update(math::point3* position, const math::quat& rotation, const real delta, const real speed);
		private:
			bool forwardState;
			bool backwardState;
			bool leftState;
			bool rightState;
			bool upState;
			bool downState;
		};
	}
}

#endif
