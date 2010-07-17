#include <pwn/render/compiledcamera.h>
#include <pwn/render/camera.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		CompiledCamera::CompiledCamera(const Camera& camera)
			: mat( cmat44(-camera.orientation) * cmat44(-camera.position.vec) )
		{
		}
	}
}