// pwn/StringUtils

#ifndef PWN_STRINGUTILS
#define PWN_STRINGUTILS

#include <pwn/string>
#include <vector>

namespace pwn
{
	namespace core
	{
		const pwn::string& kSpaceCharacters();

		void SplitString(const pwn::string& stringToSplit, std::vector<pwn::string>* result, const pwn::string& delimiterString);
		std::vector<pwn::string> SplitString(const pwn::string& stringToSplit, const pwn::string& delimiterString);
		pwn::string TrimRight(const pwn::string& stringToTrim,const pwn::string& trimCharacters=kSpaceCharacters());
		pwn::string TrimLeft(const pwn::string& stringToTrim,const pwn::string& trimCharacters=kSpaceCharacters());
		pwn::string Trim(const pwn::string& stringToTrim,const pwn::string& trimCharacters=kSpaceCharacters());
		bool StartsWith(const pwn::string stringToTest, const pwn::string& start);
		bool EndsWith(const pwn::string stringToTest, const pwn::string& end);
		pwn::string ToLower(const pwn::string& string);
		void StringReplace(pwn::string* string, const pwn::string& toFind, const pwn::string& toReplace);
		const pwn::string StringReplace(const pwn::string& string, const pwn::string& toFind, const pwn::string& toReplace);
	}
}

#endif