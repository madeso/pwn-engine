#ifndef PWN_RENDER_CAMERA
#define PWN_RENDER_CAMERA

#include <pwn/math/types.h>
#include <pwn/render/fse/types.h>
namespace pwn
{
	namespace render
	{
		/** A camera in World3.
		 */
		class Camera
		{
		public:
			Camera(math::point3 position, math::quat orientation, real fov, real znear, real zfar);

			math::point3 position;
			math::quat orientation;
			real fov;
			real znear;
			real zfar;

			fse::PipelinePtr pipeline;
		};
	}
}

#endif