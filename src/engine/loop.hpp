#ifndef PWN_ENGINE_LOOP_HPP
#define PWN_ENGINE_LOOP_HPP

namespace pwn
{
namespace engine
{
    class Loop;

    namespace ActiveLoop
    {
        Loop&
        Get();
        bool
        Has();
    }
}
}

#endif
