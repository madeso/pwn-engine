#ifndef PWN_RENDER_FSE_PROVIDER_PROVIDERS
#define PWN_RENDER_FSE_PROVIDER_PROVIDERS

#include <pwn/string.h>
#include <pwn/render/fse/types.h>
#include <pwn/core/config.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			ProviderPtr Providers_Create(const string& name, const core::Ptree& x, const string& file);
		}
	}
}

#endif
