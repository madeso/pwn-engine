#ifndef PWN_ENGINE_SYSTEM_VFS_HPP
#define PWN_ENGINE_SYSTEM_VFS_HPP

#include <pwn/string.h>

namespace pwn
{
namespace engine
{
    class Game;
    void
    SystemInstall_Vfs(
            Game* game,
            const pwn::string& argv0,
            const pwn::string& company,
            const pwn::string& app);
}
}

#endif
