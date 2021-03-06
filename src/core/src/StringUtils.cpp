#ifdef _MSC_VER
// 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <pwn/core/stringutils.h>

#include <pwn/assert.h>
#include <boost/tokenizer.hpp>
#include <boost/smart_ptr.hpp>
#include <cstring>

namespace pwn
{
namespace core
{
    const pwn::string&
    kSpaceCharacters()
    {
        static const pwn::string space = " \n\r\t";
        return space;
    }

    void
    SplitString(
            const pwn::string& stringToSplit,
            std::vector<pwn::string>* result,
            const pwn::string& delimiterString)
    {
        Assert(result);
        boost::char_separator<pwn::tchar> sep(delimiterString.c_str());
        boost::tokenizer<boost::char_separator<pwn::tchar>> tok(
                stringToSplit,
                sep);
        std::copy(tok.begin(), tok.end(), std::back_inserter(*result));
    }

    std::vector<pwn::string>
    SplitString(
            const pwn::string& stringToSplit,
            const pwn::string& delimiterString)
    {
        std::vector<pwn::string> temp;
        SplitString(stringToSplit, &temp, delimiterString);
        return temp;
    }

    pwn::string
    TrimRight(
            const pwn::string& stringToTrim,
            const pwn::string& trimCharacters)
    {
        return pwn::string(stringToTrim)
                .erase(stringToTrim.find_last_not_of(trimCharacters) + 1);
    }

    pwn::string
    TrimLeft(const pwn::string& stringToTrim, const pwn::string& trimCharacters)
    {
        return pwn::string(stringToTrim)
                .erase(0, stringToTrim.find_first_not_of(trimCharacters));
    }

    pwn::string
    Trim(const pwn::string& stringToTrim, const pwn::string& trimCharacters)
    {
        return TrimRight(
                TrimLeft(stringToTrim, trimCharacters),
                trimCharacters);
    }

    bool
    StartsWith(const pwn::string stringToTest, const pwn::string& start)
    {
        const pwn::string::size_type length = start.length();
        const pwn::string::size_type otherLength = stringToTest.length();
        if (otherLength < length)
        {
            return false;
        }
        const pwn::string actualStart = stringToTest.substr(0, length);
        return start == actualStart;
    }
    bool
    EndsWith(const pwn::string stringToTest, const pwn::string& end)
    {
        const pwn::string::size_type length = end.length();
        const pwn::string::size_type otherLength = stringToTest.length();
        if (otherLength < length)
        {
            return false;
        }
        const pwn::string actualEnd =
                stringToTest.substr(otherLength - length, length);
        return end == actualEnd;
    }

    pwn::string
    ToLower(const pwn::string& string)
    {
        pwn::string result = string;
        //std::for_each(result.begin(), result.end(), tolower);
        std::transform(result.begin(), result.end(), result.begin(), tolower);
        return result;
    }

    void
    StringReplace(
            pwn::string* string,
            const pwn::string& toFind,
            const pwn::string& toReplace)
    {
        std::size_t index = string->find(toFind);
        const std::size_t findLength = toFind.length();
        while (index != pwn::string::npos)
        {
            string->erase(index, findLength);
            string->insert(index, toReplace);
            index = string->find(toFind, index);
        }
    }

    const pwn::string
    StringReplace(
            const pwn::string& string,
            const pwn::string& toFind,
            const pwn::string& toReplace)
    {
        pwn::string temp = string;
        StringReplace(&temp, toFind, toReplace);
        return temp;
    }

    void
    Copy(pwn::tchar* dst,
         const pwn::string& src,
         const pwn::string::size_type count)
    {
        strncpy(dst, src.c_str(), count - 1);
        dst[count - 1] = 0;
    }
}
}
