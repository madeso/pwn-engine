#ifndef PWN_RENDER_FSE_TARGET_TARGETS
#define PWN_RENDER_FSE_TARGET_TARGETS

#include <pwn/render/fse/types.h>
#include <pwn/core/config.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			TargetPtr Targets_Create(const string& name, const core::Ptree& data);
		}
	}
}

#endif
