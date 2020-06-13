#include <pwn/core/enum.h>
#include <pwn/assert.h>

namespace pwn
{
namespace core
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Enum Type

    EnumType::EnumType() : isAdding(true), nextIndex(0)
    {
    }

    EnumType::~EnumType()
    {
        Assert(isAdding == false);
        Assert(createdButNotAddedList.empty() == true);
    }

    const string&
    EnumType::toString(size_t v) const
    {
        Assert(v < nextIndex);
        List::const_iterator f = list.find(v);
        if (f != list.end())
        {
            return f->second;
        }
        else
        {
            List::const_iterator i = createdButNotAddedList.find(v);
            if (i == createdButNotAddedList.end())
            {
                throw "unknown index";
            }
            else
            {
                return i->second;
            }
        }
    }

    const EnumValue
    EnumType::toEnum(const string& name)
    {
        Map::const_iterator r = map.find(name);
        if (r == map.end())
        {
            if (isAdding)
            {
                const size_t id = nextIndex;
                ++nextIndex;
                createdButNotAddedList.insert(List::value_type(id, name));
                createdButNotAddedMap.insert(Map::value_type(name, id));
                map.insert(Map::value_type(name, id));
                return EnumValue(this, id);
            }
            else
            {
                throw "loading has finished, enum doesnt exist or is misspelled";
            }
        }
        else
        {
            return EnumValue(this, r->second);
        }
    }

    void
    EnumType::addEnum(const string& name)
    {
        Assert(isAdding == true);
        Map::const_iterator r = map.find(name);
        if (r == map.end())
        {
            const size_t id = nextIndex;
            ++nextIndex;
            list.insert(List::value_type(id, name));
            map.insert(Map::value_type(name, id));
        }
        else
        {
            Map::const_iterator f = createdButNotAddedMap.find(name);
            if (f == createdButNotAddedMap.end())
            {
                throw "enum already added";
            }
            else
            {
                // move to list
                const size_t id = f->second;
                List::const_iterator i = createdButNotAddedList.find(id);
                Assert(i !=
                       createdButNotAddedList
                               .end());  // createdButNotAdded list/map inconsistencies
                createdButNotAddedList.erase(i);
                createdButNotAddedMap.erase(f);
            }
        }
    }

    void
    EnumType::stopAdding()
    {
        Assert(isAdding == true);
        isAdding = false;
        Assert(createdButNotAddedList.empty() ==
               true);  // if this isn't empty, some enums have not been added or misspelling has occured, see throw above
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Enum Value

    EnumValue::EnumValue(EnumType* t, size_t v) : type(t), value(v)
    {
    }

    const string
    EnumValue::toString() const
    {
        return type->toString(value);
    }

    const size_t
    EnumValue::toValue() const
    {
        return value;
    }

    bool
    EnumValue::operator==(const EnumValue& other) const
    {
        Assert(type == other.type);
        return value == other.value;
    }
    bool
    EnumValue::operator!=(const EnumValue& other) const
    {
        return !(*this == other);
    }
    bool
    EnumValue::operator<(const EnumValue& other) const
    {
        Assert(type == other.type);
        return value < other.value;
    }
    std::ostream&
    operator<<(std::ostream& s, const EnumValue& v)
    {
        s << v.toString();
        return s;
    }
}
}
