#include <pwn/render/compiledcamera.h>
#include <pwn/render/camera.h>
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		using namespace pwn::math;

		math::mat44 G()
		{
			//return cmat44(RightHandAround(Up(), Angle::FromDegrees(180)));
			math::mat44 m;
			cml::matrix_scale(m, 1.f, 1.f, -1.f);
			return m;
		}

		const math::mat44 fixOpenGlNegativeZ = G();

		CompiledCamera::CompiledCamera(const Camera& camera)
			// : mat( fixOpenGlNegativeZ * cmat44(vec3(-X(camera.position.vec), -Y(camera.position.vec), Z(camera.position.vec))) * cmat44(-camera.rotation) )
			: mat( fixOpenGlNegativeZ * cmat44(vec3(-camera.position.vec)) * cmat44(camera.rotation) )
		{
		}

		math::mat44 CompiledCamera::generateMatrix(const math::vec3& pos, const math::quat& rot) const
		{
			return mat * math::cmat44(pos) * math::cmat44(rot);
		}
	}
}
