#define PWN_ENGINE_NO_AUTOLINK

#include <pwn/engine/loop.h>
#include <pwn/engine/game.h>
#include "gameimp.hpp"
#include "loop.hpp"

#include <pwn/sfml-config.h>
#include <SFML/System/Clock.hpp>

namespace pwn
{
namespace engine
{
    Loop::Loop(Game* game) : isRunning_(false), game_(game)
    {
    }

    Loop::~Loop()
    {
    }

    Game&
    Loop::game()
    {
        if (game_ == 0)
        {
            throw "invalid game variable";
        }
        return *game_;
    }

    void
    Loop::stop()
    {
        isRunning_ = false;
    }

    bool
    Loop::isRunning() const
    {
        return isRunning_;
    }

    namespace  // local
    {
        Loop*&
        InnerMostLoop()
        {
            static Loop* loop = 0;
            return loop;
        }

        class RaiiInnerMostLoop
        {
        public:
            explicit RaiiInnerMostLoop(Loop* me) : old(InnerMostLoop())
            {
                InnerMostLoop() = me;
            }
            ~RaiiInnerMostLoop()
            {
                InnerMostLoop() = old;
            }

        private:
            Loop* old;
        };
    }

    namespace ActiveLoop
    {
        Loop&
        Get()
        {
            Loop* loop = InnerMostLoop();
            if (loop == 0)
            {
                throw "assertion";
            }
            return *loop;
        }
        bool
        Has()
        {
            return InnerMostLoop() != 0;
        }
    }

    void
    Loop::loop()
    {
        RaiiInnerMostLoop raiiInnerMostLoop(this);
        isRunning_ = true;
        sf::Clock clock;
        while (isRunning())
        {
            const real delta = clock.GetElapsedTime();
            clock.Reset();
            update(delta);
            render();
        }
    }

    void
    Loop::update(real delta)
    {
        if (isRunning() == false)
        {
            return;
        }
        game().getImp().updateSystems();

        if (isRunning() == false)
        {
            return;
        }
        onUpdate(delta);
    }

    void
    Loop::render()
    {
        if (isRunning() == false)
        {
            return;
        }
        onRender();
    }

    void
    Loop::renderDisplay(render::VirtualDisplay& display, int id)
    {
        game().getImp().display(id, display);
    }

    void
    Loop::defaultRender()
    {
        renderDisplay(display);
    }
}
}
