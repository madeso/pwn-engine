#ifndef PWN_RENDER_FSE_COMMAND_BINDBUFFERCOMMAND
#define PWN_RENDER_FSE_COMMAND_BINDBUFFERCOMMAND

#include <boost/utility.hpp>
#include <pwn/render/fse/Command.h>
#include <pwn/core/config.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class BindBufferCommand
				: public Command
				, boost::noncopyable
			{
			public:
				BindBufferCommand(const core::Ptree& el, Provider* prov);
				~BindBufferCommand();

				void apply();
				std::vector<Provider*> getDependencies();
				const string toString() const;

			protected:
				void doLink(Linker* user);
				void doBind(Binder* bd);

			private:
				const int location;
				const string name;
				BufferReferencePtr buffer;
				TargetPtr targ;
			};
		}
	}
}

#endif
