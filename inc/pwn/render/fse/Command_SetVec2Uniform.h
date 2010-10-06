#ifndef PWN_RENDER_FSE_COMMAND_SETVEC2UNIFORM
#define PWN_RENDER_FSE_COMMAND_SETVEC2UNIFORM

#include <pwn/core/config.h>
#include <pwn/render/fse/Command.h>
#include <boost/utility.hpp>
#include <pwn/math/types.h>
#include <pwn/render/shader.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class SetVec2Uniform
				: public Command
				, boost::noncopyable
			{
			public:
				SetVec2Uniform(const core::Ptree& el, Provider* p);
				~SetVec2Uniform();
				void apply();
				std::vector<Provider*> getDependencies();

			protected:
				void doLink(Linker* user);
				void doBind(Binder* bd);
				
			private:
				const string shaderName;
				const string uniformName;
				const math::vec2 vec;
				Provider* provider;
				UniformPtr uniform;
			};
		}
	}
}

#endif
