#include <pwn/engine/democamera>
#include <pwn/math/operations>
#include <pwn/render/world3widget>

namespace pwn
{
	namespace engine
	{
		DemoCamera::DemoCamera()
			: camera(math::Origo3(), math::qIdentity(), 45, 0.5f, 1000.0f)
			, forward(Key::W)
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

		namespace // local
		{
			int multi(bool pos, bool neg)
			{
				int v = 0;
				if( pos ) ++v;
				if( neg ) --v;
				return v;
			}
		}

		void DemoCamera::update(const real delta, const real speed)
		{
			using namespace pwn::math;
			const vec3 movement = multi(forwardState, backwardState) * In(camera.orientation)
				+ multi(rightState, leftState) * Right(camera.orientation)
				+ multi(upState, downState) * Up();
			camera.position.vec += movement * delta * speed;


			camera.orientation = math::qLookAtOrNot(camera.position.vec, vec3(0,0,0), Up());

			world->setCamera(camera);
		}
	}
}