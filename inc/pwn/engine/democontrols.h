#ifndef PWN_ENGINE_DEMOCONTROLS
#define PWN_ENGINE_DEMOCONTROLS

#include <pwn/engine/key.h>
#include <pwn/math/types.h>
#include <pwn/engine/demomovement.h>

namespace pwn
{
	namespace engine
	{
		class DemoControls : DemoMovement
		{
		public:
			DemoControls();
			~DemoControls();

			void onMouse(const math::vec2& movement);
			void update(math::point3* position, math::quat* rotation, const real delta, const real speed, const real sensitivity);
		private:
			math::vec2 mouse;
		};
	}
}

#endif
