#ifndef PWN_CONFIG
#define PWN_CONFIG

namespace pwn
{
	// flaots
	typedef float float32;

	// integers too...
#ifdef _MSC_VER
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#else
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

	typedef signed int int32;
	typedef signed short int16;
	typedef signed char int8;

	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	typedef int8 byte;
	typedef uint8 ubyte;

	// and finally real
	typedef float32 real;
#define PWN_MATH_VALUE(x) x##f
}

#endif
