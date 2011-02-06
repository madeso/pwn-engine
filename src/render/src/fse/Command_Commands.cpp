#include <pwn/render/fse/Command_Commands.h>
#include <pwn/render/fse/Command_BindBufferCommand.h>
#include <pwn/render/fse/Command_SetVec2Uniform.h>
#include <pwn/render/fse/exceptions.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			CommandPtr Commands_Create(const string& name, const core::Ptree& el, Provider* pro)
			{
				if (name == "bindbuffer") return CommandPtr(new BindBufferCommand(el, pro));
				else if (name == "setu2") return CommandPtr(new SetVec2Uniform(el, pro));
				else throw FseException(name + " is not a valid command");
			}
		}
	}
}
