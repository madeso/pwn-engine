#include <pwn/render/compiledcamera.h>
#include <pwn/render/camera.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		using namespace pwn::math;

		CompiledCamera::CompiledCamera(const Camera& camera)
			: mat( cmat44(-camera.orientation) * cmat44(RightHandAround(Up(), Angle::FromDegrees(180))) * cmat44(vec3(-X(camera.position.vec), -Y(camera.position.vec), Z(camera.position.vec))) )
		{
		}

		math::mat44 CompiledCamera::generateMatrix(const math::vec3& pos, const math::quat& rot) const
		{
			return mat * math::cmat44(pos) * math::cmat44(rot);
		}
	}
}
