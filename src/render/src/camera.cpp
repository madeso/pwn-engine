#include <pwn/render/camera.h>
#include <pwn/math/operations.h>

namespace pwn
{
namespace render
{
    Camera::Camera(
            const math::point3& position,
            const math::quat& rotation,
            real fov,
            real znear,
            real zfar)
        : position(position)
        , rotation(rotation)
        , fov(fov)
        , znear(znear)
        , zfar(zfar)
    {
    }

    void
    Camera::lookAt(const math::point3& p)
    {
        rotation = math::qLookAt(position.vec, p.vec, math::Up());
    }
}
}
