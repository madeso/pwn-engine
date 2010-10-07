﻿#include <pwn/render/fse/Provider_Providers.h>
#include <pwn/render/fse/Provider_RenderFullscreenProvider.h>
#include <pwn/render/fse/Provider_RenderWorldProvider.h>
#include <pwn/render/fse/exceptions.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			ProviderPtr Providers_Create(const string& name, const core::Ptree& x)
			{
				if (name == "fullscreen")
				{
					return ProviderPtr(new RenderFullscreenProvider(x));
				}
				else if (name == "world")
				{
					return ProviderPtr(new RenderWorldProvider(x));
				}
				else throw FseException(name + " is not a known provider");
			}
		}
	}
}