#include <pwn/component/componentdef.h>
#include <pwn/component/property.h>

namespace pwn
{
namespace component
{
    ComponentArgs::ComponentArgs()
    {
    }

    ComponentArgs::~ComponentArgs()
    {
    }

    void
    ComponentArgs::add(const string& s, PropertyPtr prop)
    {
        vars.insert(Vars::value_type(s, prop));
    }

    ComponentArgs::PropertyPtr
    ComponentArgs::get(const string& s) const
    {
        Vars::const_iterator f = vars.find(s);
        if (f == vars.end())
        {
            throw "unknown component argument";
        }
        return f->second;
    }
}
}
