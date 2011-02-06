#include <pwn/render/fse/Command_SetVec2Uniform.h>
#include <pwn/render/fse/Binder.h>
#include <pwn/render/fse/Provider.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			SetVec2Uniform::SetVec2Uniform(const core::Ptree& el, Provider* p)
				: Command(el, p)
				, uniformName( el.get<string>("uniform") )
				, vec( el.get("x", 0.0f), el.get("y", 0.0f) )
				, provider(p)
			{
			}

			SetVec2Uniform::~SetVec2Uniform()
			{
			}

			void SetVec2Uniform::apply()
			{
				uniform->bindUniform(vec);
			}

			std::vector<Provider*> SetVec2Uniform::getDependencies()
			{
				return Command::NoDependencies;
			}

			void SetVec2Uniform::doLink(Linker* user)
			{
			}

			void SetVec2Uniform::doBind(Binder* bd)
			{
				ShaderPtr shader = provider->getShader();
				uniform = shader->getUniform(uniformName);
			}
		}
	}
}
