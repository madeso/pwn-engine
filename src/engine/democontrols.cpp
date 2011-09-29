#include <pwn/engine/democontrols.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace engine
	{
		DemoControls::DemoControls()
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

		DemoControls::~DemoControls()
		{
		}

		bool DemoControls::onKey(Key::Code key, bool newState)
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

		void DemoControls::onMouse(const math::vec2 movement)
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

		void DemoControls::update(math::point3* position, math::quat* rotation, const real delta, const real speed, const real sensitivity)
		{
			using namespace pwn::math;
			const vec3 movement = multi(forwardState, backwardState) * In(*rotation)
				+ multi(rightState, leftState) * Right(*rotation)
				+ multi(upState, downState) * Up();
			position->vec += movement * delta * speed;

			const math::quat updown = math::cquat(math::RightHandAround(Right(), Angle::FromDegrees(Y(mouse)*sensitivity)));
			const math::quat rightleft = math::cquat( math::RightHandAround(Up(), Angle::FromDegrees(-X(mouse)*sensitivity)));
			
			//*rotation = math::Combine_Local(*rotation, rightleft);
			*rotation = math::Combine_Parent(*rotation, updown);

			mouse = vec2(0,0);
		}
	}
}
