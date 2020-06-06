#include <pwn/engine/democontrols.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace engine
	{
		DemoControls::DemoControls()
			: mouse(0, 0)
		{
		}

		DemoControls::~DemoControls()
		{
		}

		void DemoControls::onMouse(const math::vec2& movement)
		{
			mouse += movement;
		}

		void DemoControls::update(math::point3* position, math::quat* rotation, const real delta, const real speed, const real sensitivity)
		{
			DemoMovement::update(position, *rotation, delta, speed);
			using namespace pwn::math;

			const math::quat updown = math::cquat(math::RightHandAround(Right(), Angle::FromDegrees(Y(mouse) * sensitivity)));
			const math::quat rightleft = math::cquat(math::RightHandAround(Up(), Angle::FromDegrees(-X(mouse) * sensitivity)));

			//*rotation = math::Combine_Local(*rotation, rightleft);
			*rotation = math::Combine_Parent(*rotation, updown);

			mouse = vec2(0, 0);
		}
	}
}
