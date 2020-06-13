#define PWN_ENGINE_NO_AUTOLINK

#include <algorithm>
#include <pwn/engine/videomode.h>
#include <pwn/sfml-config.h>
#include <SFML/Window.hpp>

namespace pwn
{
namespace engine
{
    VideoMode::VideoMode()
    {
    }

    VideoMode::VideoMode(int width, int height, int bits, bool fullscreen)
        : width(width)
        , height(height)
        , bits(bits)
        , fullscreen(fullscreen)
    {
    }

    namespace  // local
    {
        const sf::VideoMode
        ToSfml(const VideoMode& v)
        {
            return sf::VideoMode(v.width, v.height, v.bits);
        }

        const VideoMode
        ToPwn(const sf::VideoMode& v)
        {
            return VideoMode(v.Width, v.Height, v.BitsPerPixel, false);
        }
    }

    bool
    VideoMode::isValid() const
    {
        return ToSfml(*this).IsValid();
    }

    namespace VideoModes
    {
        const std::vector<VideoMode>
        List()
        {
            const std::vector<sf::VideoMode>& m =
                    sf::VideoMode::GetFullscreenModes();
            std::vector<VideoMode> r(m.size());
            std::transform(m.begin(), m.end(), r.begin(), ToPwn);
            return r;
        }

        const VideoMode
        Desktop()
        {
            return ToPwn(sf::VideoMode::GetDesktopMode());
        }
    }
}
}
