#include <pwn/render/compiledcamera>
#include <pwn/render/camera>
#include <pwn/math/operations>

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