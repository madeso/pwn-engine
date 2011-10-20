#ifndef PWN_RENDER_FSE_COMMAND_COMMANDS
#define PWN_RENDER_FSE_COMMAND_COMMANDS

#include <pwn/string.h>
#include <pwn/render/fse/types.h>
#include <pwn/core/config.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			CommandPtr Commands_Create(const string& name, const core::Ptree& el, Provider* pro);
		}
	}
}

#endif
