#define PWN_ENGINE_NO_AUTOLINK

#include "system_vfs.hpp"

#include "system.hpp"
#include <pwn/engine/game.h>
#include <pwn/assert.h>

#include <boost/filesystem.hpp>
#include <iostream>

namespace pwn
{
namespace engine
{
    void
    SystemInstall_Vfs(
            Game* game,
            const pwn::string& argv0,
            const pwn::string& company,
            const pwn::string& app)
    {
    }
}
}
