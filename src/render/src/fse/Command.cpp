#include <pwn/render/fse/Command.h>
#include <pwn/render/fse/Provider.h>

namespace pwn
{
namespace render
{
    namespace fse
    {
        Command::Command(const core::Ptree& el, Provider* aprov)
            : id(el.get<string>("id"))
            , prov(aprov)
        {
        }

        Command::~Command()
        {
        }

        void
        Command::link(Linker* linker)
        {
            doLink(linker);
        }

        void
        Command::bind(Binder* b)
        {
            doBind(b);
        }

        const string
        Command::toString() const
        {
            return id;
        }

        BufferReferencePtr
        Command::createBuffer(const string& name)
        {
            return prov->createBuffer(name);
        }

        const std::vector<Provider*> Command::NoDependencies;
    }
}
}
