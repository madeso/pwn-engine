#include <pwn/render/camera>

namespace pwn
{
	namespace render
	{
		Camera::Camera(math::point3 position, math::quat orientation, real fov, real znear, real zfar)
			: position(position)
			, orientation(orientation)
			, fov(fov)
			, znear(znear)
			, zfar(zfar)
		{
		}
	}
}