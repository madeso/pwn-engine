#include <pwn/engine/democontrols.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace engine
	{
		DemoCamera::DemoCamera()
			: forward(Key::W)
			, backward(Key::S)
			, left(Key::A)
			, right(Key::D)
			, up(Key::Space)
			, down(Key::LControl)
			, forwardState(false)
			, backwardState(false)
			, leftState(false)
			, rightState(false)
			, upState(false)
			, downState(false)
			, mouse(0, 0)
		{
		}

		DemoCamera::~DemoCamera()
		{
		}

		bool DemoCamera::onKey(Key::Code key, bool newState)
		{
#define HANDLEKEY(k) if( k == key ) { k##State=newState; return true; }
			      HANDLEKEY(forward )
			else HANDLEKEY( backward )
			else HANDLEKEY( left )
			else HANDLEKEY( right )
			else HANDLEKEY( up )
			else HANDLEKEY( down )
#undef HANDLEKEY
			else return false;
		}

		void DemoCamera::onMouse(const math::vec2 movement)
		{
			mouse += movement;
		}

		namespace // local
		{
			pwn::real multi(bool pos, bool neg)
			{
				pwn::real v = 0;
				if( pos ) v+=1;
				if( neg ) v-=1;
				return v;
			}
		}

		void DemoCamera::update(math::point3* position, math::quat* orientation, const real delta, const real speed, const real sensitivity)
		{
			using namespace pwn::math;
			const vec3 movement = multi(forwardState, backwardState) * In(camera.orientation)
				+ multi(rightState, leftState) * Right(camera.orientation)
				+ multi(upState, downState) * Up();
			position->vec += movement * delta * speed;

			const math::quat updown = math::cquat(math::RightHandAround(Right(), Angle::FromDegrees(Y(mouse)*sensitivity)));
			const math::quat rightleft = math::cquat( math::RightHandAround(Up(), Angle::FromDegrees(-X(mouse)*sensitivity)));
			
			//*orientation = math::Combine_Local(*orientation, rightleft);
			*orientation = math::Combine_Parent(*orientation, updown);

			mouse = vec2(0,0);
		}
	}
}
