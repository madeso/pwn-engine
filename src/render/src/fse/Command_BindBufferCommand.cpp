#include <pwn/render/fse/Command_BindBufferCommand.h>
#include <pwn/render/fse/BufferReference.h>
#include <pwn/render/fse/Target.h>
#include <pwn/core/str.h>
#include <pwn/render/fse/Linker.h>
#include <pwn/render/fse/Binder.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			BindBufferCommand::BindBufferCommand(const core::Ptree& el, Provider* prov)
				: Command(el, prov)
				, location( el.get("location", -1) )
				, name( el.get<string>("buffer") )
			{
			}

			BindBufferCommand::~BindBufferCommand()
			{
			}

			void BindBufferCommand::apply()
			{
				buffer->bindTexture(location);
			}

			std::vector<Provider*> BindBufferCommand::getDependencies()
			{
				std::vector<Provider*> ret;
				ret.push_back(targ->getProvider());
				return ret;
			}

			const string BindBufferCommand::toString() const
			{
				return core::Str() << Command::toString() << " who binds buffer " << name << " to " << location;
			}

			void BindBufferCommand::doLink(Linker* user)
			{
				buffer = createBuffer(name);
				targ = user->getTarget(name);
			}

			void BindBufferCommand::doBind(Binder* bd)
			{
				bd->reference(buffer);
			}
		}
	}
}
