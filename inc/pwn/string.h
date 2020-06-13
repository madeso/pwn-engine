// inc/pwn

#ifndef PWN_STRING
#define PWN_STRING

#include <string>
#include <sstream>

namespace pwn
{
typedef std::string string;
typedef std::ostringstream ostringstream;
typedef std::istringstream istringstream;
typedef char tchar;
typedef unsigned int tchari;  // number large enough to hold tchar
}


#endif
