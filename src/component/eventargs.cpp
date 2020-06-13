#include <pwn/component/eventargs.h>
#include <pwn/component/property.h>

namespace pwn
{
namespace component
{
    EventArgs::EventArgs()
    {
    }

    EventArgs::~EventArgs()
    {
    }

    void
    EventArgs::add(boost::shared_ptr<Property> prop)
    {
        list.push_back(prop);
    }

    EventArgs&
    EventArgs::operator<<(boost::shared_ptr<Property> prop)
    {
        add(prop);
        return *this;
    }

    size_t
    EventArgs::count() const
    {
        return list.size();
    }

    core::EnumType&
    EventArgs::Type()
    {
        static core::EnumType type;
        return type;
    }
}
}
