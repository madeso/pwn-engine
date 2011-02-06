#ifndef PWN_RENDER_FSE_LINKER
#define PWN_RENDER_FSE_LINKER

#include <pwn/render/fse/types.h>
#include <pwn/core/map.h>
#include <pwn/render/Shader.h>

namespace pwn
{
	namespace render
	{
		class ShaderPool;

		namespace fse
		{
			class Linker
			{
			public:
				Linker();
				~Linker();

				void addProvider(ProviderPtr prov);
				void addTarget(TargetPtr targ);
				TargetPtr getTarget(string name);
				ProviderPtr getProvider(const string& name);
				void storePipeline(PipelinePtr get, TargetPtr target);
				void link();
				ShaderPtr getShaderOrNull(const string& id);

			public:
				/// @return the suggested target name
				const string read(const string& path, ShaderPool* shaders);

			private:
				core::Map<ProviderPtr> providers;
				core::Map<TargetPtr> targets;
				core::Map<ShaderPtr> shaders;
			};
		}
	}
}

#endif
