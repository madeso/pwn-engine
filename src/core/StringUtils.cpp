#include <pwn/core/StringUtils>

#include <cassert>
#include <boost/tokenizer.hpp>
#include <boost/smart_ptr.hpp>

namespace pwn
{
	namespace core
	{
		const pwn::core::string& kSpaceCharacters()
		{
			static const pwn::core::string space = " \n\r\t";
			return space;
		}

		void SplitString(const pwn::core::string& stringToSplit, std::vector<pwn::core::string>* result, const pwn::core::string& delimiterString)
		{
			assert(result);
			boost::char_separator<pwn::core::tchar> sep(delimiterString.c_str());
			boost::tokenizer< boost::char_separator<pwn::core::tchar> > tok(stringToSplit, sep);
			std::copy(tok.begin(), tok.end(), std::back_inserter(*result));
		}

		std::vector<pwn::core::string> SplitString(const pwn::core::string& stringToSplit, const pwn::core::string& delimiterString)
		{
			std::vector<pwn::core::string> temp;
			SplitString(stringToSplit, &temp, delimiterString);
			return temp;
		}

		pwn::core::string TrimRight(const pwn::core::string& stringToTrim,const pwn::core::string& trimCharacters)
		{
			return pwn::core::string(stringToTrim).erase(stringToTrim.find_last_not_of(trimCharacters)+1);
		}

		pwn::core::string TrimLeft(const pwn::core::string& stringToTrim,const pwn::core::string& trimCharacters)
		{
			return pwn::core::string(stringToTrim).erase(0,stringToTrim.find_first_not_of(trimCharacters));
		}

		pwn::core::string Trim(const pwn::core::string& stringToTrim,const pwn::core::string& trimCharacters)
		{
			return TrimRight( TrimLeft(stringToTrim, trimCharacters), trimCharacters);
		}

		bool StartsWith(const pwn::core::string stringToTest, const pwn::core::string& start)
		{
			const pwn::core::string::size_type length = start.length();
			const pwn::core::string::size_type otherLength = stringToTest.length();
			if( otherLength < length ) return false;
			const pwn::core::string actualStart = stringToTest.substr(0, length);
			return start == actualStart;
		}
		bool EndsWith(const pwn::core::string stringToTest, const pwn::core::string& end)
		{
			const pwn::core::string::size_type length = end.length();
			const pwn::core::string::size_type otherLength = stringToTest.length();
			if( otherLength < length ) return false;
			const pwn::core::string actualEnd = stringToTest.substr(otherLength-length, length);
			return end == actualEnd;
		}

		pwn::core::string ToLower(const pwn::core::string& string)
		{
			pwn::core::string result = string;
			//std::for_each(result.begin(), result.end(), tolower);
			std::transform(result.begin(), result.end(), result.begin(), tolower);
			return result;
		}

		void StringReplace(pwn::core::string* string, const pwn::core::string& toFind, const pwn::core::string& toReplace)
		{
			std::size_t index = string->find(toFind);
			const std::size_t findLength = toFind.length();
			while (index!=pwn::core::string::npos) {
				string->erase(index, findLength);
				string->insert(index, toReplace);
				index = string->find(toFind, index);
			}
		}

		const pwn::core::string StringReplace(const pwn::core::string& string, const pwn::core::string& toFind, const pwn::core::string& toReplace)
		{
			pwn::core::string temp = string;
			StringReplace(&temp, toFind, toReplace);
			return temp;
		}
	}
}