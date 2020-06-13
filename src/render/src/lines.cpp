#include <pwn/render/lines.h>
#include <pwn/math/rgb.h>
#include <pwn/render/renderlist.h>
#include <pwn/math/operations.h>
#include <pwn/render/compiledcamera.h>

//#pragma warning (disable:4512) // boost\utility\addressof.hpp(30) : warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
namespace render
{
    Lines::Lines(real w)
        : color(math::Rgb::From(math::Color::White), 1)
        , width(w)
    {
    }

    boost::shared_ptr<Lines>
    Lines::Create(real w)
    {
        boost::shared_ptr<Lines> l(new Lines(w));
        return l;
    }

    void
    Lines::render(RenderList* rl, const CompiledCamera& cam)
    {
        BOOST_FOREACH (const Line& l, lines)
        {
            rl->add(width,
                    color,
                    math::cvec3(cam.generateMatrix(l.from)),
                    math::cvec3(cam.generateMatrix(l.to)));
        }
    }

    Lines::Line::Line(const math::vec3& f, const math::vec3& t) : from(f), to(t)
    {
    }

    void
    Lines::add(const math::vec3& from, const math::vec3& to)
    {
        lines.push_back(Line(from, to));
    }
}
}
