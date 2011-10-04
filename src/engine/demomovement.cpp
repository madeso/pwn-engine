#include <pwn/engine/demomovement.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace engine
	{
		DemoMovement::DemoMovement()
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
			, localUp(false)
		{
		}

		DemoMovement::~DemoMovement()
		{
		}

		bool DemoMovement::onKey(Key::Code key, bool newState)
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

		void DemoMovement::update(math::point3* position, const math::quat& rotation, const real delta, const real speed)
		{
			using namespace pwn::math;
			const vec3 movement = multi(forwardState, backwardState) * In(rotation)
				+ multi(rightState, leftState) * Right(rotation)
				+ multi(upState, downState) * (localUp?Up(rotation):Up());
			position->vec += movement * delta * speed;
		}
	}
}
