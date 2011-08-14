#include <pwn/render/compiledcamera.h>
#include <pwn/render/camera.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		using namespace pwn::math;

		CompiledCamera::CompiledCamera(const Camera& camera)
			: mat( cmat44(-camera.orientation) * cmat44(vec3(-X(camera.position.vec), -Y(camera.position.vec), Z(camera.position.vec))) )
		{
		}
	}
}
