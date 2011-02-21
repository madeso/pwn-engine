#include <pwn/render/compiledcamera.h>
#include <pwn/render/camera.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		CompiledCamera::CompiledCamera(const Camera& camera)
			: mat( math::cmat44(-camera.orientation) * math::cmat44(static_cast<math::vec3>(-camera.position.vec)) )
		{
		}
	}
}
