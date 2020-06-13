#include <pwn/render/light.h>
#include <pwn/math/operations.h>
#include "opengl_debug.hpp"

namespace pwn
{
namespace render
{
    LightProperties::LightProperties()
        : ambient(0, 0, 0, 1)
        , diffuse(0, 0, 0, 1)
        , specular(0, 0, 0, 1)
    {
    }

    void
    LightProperties::apply(int light)
    {
        glLightfv(light, GL_AMBIENT, ambient.data());
        pwnAssert_NoGLError();
        glLightfv(light, GL_DIFFUSE, diffuse.data());
        pwnAssert_NoGLError();
        glLightfv(light, GL_SPECULAR, specular.data());
        pwnAssert_NoGLError();
    }

    Attenuation::Attenuation() : constant(1), linear(0), quadratic(0)
    {
    }

    void
    Attenuation::apply(int light)
    {
        glLightf(light, GL_CONSTANT_ATTENUATION, constant);
        pwnAssert_NoGLError();
        glLightf(light, GL_LINEAR_ATTENUATION, linear);
        pwnAssert_NoGLError();
        glLightf(light, GL_QUADRATIC_ATTENUATION, quadratic);
        pwnAssert_NoGLError();
    }

    Light::~Light()
    {
        /* does nothing */
    }

    PointLight::PointLight() : position(0, 0, 0)
    {
    }

    void
    PointLight::apply(int light)
    {
        properties.apply(light);
        attenuation.apply(light);
        GLfloat pos[4] = {
                math::X(position),
                math::Y(position),
                math::Z(position),
                1};
        glLightfv(light, GL_POSITION, pos);
        pwnAssert_NoGLError();
    }


    DirectionalLight::DirectionalLight() : direction(math::qIdentity())
    {
    }

    void
    DirectionalLight::apply(int light)
    {
        properties.apply(light);
        attenuation.apply(light);
        math::vec3 vd = math::In(direction);
        GLfloat dir[4] = {math::X(vd), math::Y(vd), math::Z(vd), 0};
        glLightfv(light, GL_POSITION, dir);
        pwnAssert_NoGLError();
    }

    SpotLight::SpotLight()
        : position(0, 0, 0)
        , direction(math::qIdentity())
        , exponent(0)
        , cutoff(90)
    {
    }

    void
    SpotLight::apply(int light)
    {
        properties.apply(light);
        attenuation.apply(light);
        glLightf(light, GL_SPOT_EXPONENT, exponent);
        pwnAssert_NoGLError();
        glLightf(light, GL_SPOT_CUTOFF, cutoff);
        pwnAssert_NoGLError();
        math::vec3 vd = math::In(direction);
        GLfloat dir[3] = {math::X(vd), math::Y(vd), math::Z(vd)};
        glLightfv(light, GL_SPOT_DIRECTION, dir);
        pwnAssert_NoGLError();
        GLfloat pos[4] = {
                math::X(position),
                math::Y(position),
                math::Z(position),
                1};
        glLightfv(light, GL_POSITION, pos);
        pwnAssert_NoGLError();
    }
}
}
