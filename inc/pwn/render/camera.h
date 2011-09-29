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
			Camera(const math::point3& position, const math::quat& rotation, real fov, real znear, real zfar);
			void lookAt(const math::point3& p);

			math::point3 position;
			math::quat rotation;
			/// in degrees
			real fov;
			real znear;
			real zfar;

			fse::PipelinePtr pipeline;
		};
	}
}

#endif
