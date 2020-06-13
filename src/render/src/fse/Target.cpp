#include <pwn/render/fse/Target.h>
#include <pwn/render/fse/exceptions.h>
#include <pwn/render/fse/Provider.h>
#include <boost/foreach.hpp>
#include <pwn/render/fse/Binder.h>
#include <pwn/render/fse/BufferReference.h>

namespace pwn
{
namespace render
{
    namespace fse
    {
        Target::Target() : provider(0)
        {
        }

        Target::~Target()
        {
        }

        const string&
        Target::getId() const
        {
            return id;
        }

        void
        Target::setId(const string& aid)
        {
            if (id.empty())
            {
                id = aid;
            }
            else
            {
                FseException("Unable to change id from " + id + " to " + aid);
            }
        }

        Provider*
        Target::getProvider()
        {
            return provider;
        }

        void
        Target::setProvider(Provider* aprovider)
        {
            if (provider == 0)
            {
                provider = aprovider;
            }
            else
            {
                throw FseException(
                        "failed to set " + aprovider->toString() +
                        " as a provider for " + toString());
            }
        }

        void
        Target::bind(Binder* binder)
        {
            BOOST_FOREACH (BufferReferencePtr br, references)
            {
                binder->reference(br);
            }
            BOOST_FOREACH (AssMap::value_type k, associations)
            {
                binder->associate(k.first, k.second);
            }
        }

        BufferReferencePtr
        Target::createBuffer(const string& name)
        {
            BufferReferencePtr r(new BufferReference(name));
            references.push_back(r);
            return r;
        }

        BufferReferencePtr
        Target::createBuffer(const string& name, int width, int height)
        {
            associate(name, width, height);
            return createBuffer(name);
        }

        void
        Target::associate(const string& name, int width, int height)
        {
            associations.insert(AssMap::value_type(name, Size(width, height)));
        }

        const string
        Target::toString() const
        {
            return getId();
        }
    }
}
}
