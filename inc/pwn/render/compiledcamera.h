#ifndef PWN_RENDER_COMPILEDCAMERA
#define PWN_RENDER_COMPILEDCAMERA

#include <pwn/math/types.h>
#include <boost/noncopyable.hpp>

namespace pwn
{
	namespace render
	{
		class Camera;

		/** A camera compiled for fast transforming of actors when rendering.
		 */
		class CompiledCamera : boost::noncopyable
		{
		public:
			explicit CompiledCamera(const Camera& camera);

			const math::mat44 mat;
		};
	}
}

#endif