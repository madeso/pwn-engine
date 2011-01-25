#ifndef PWN_CONFIG
#define PWN_CONFIG

namespace pwn
{
	// flaots
	typedef float float32;

	// integers too...
	#ifdef _MSC_VER
        typedef __int64 int64_pwn;
    #else
        typedef long int64_pwn;
	#endif

	typedef int64_pwn int64;
	typedef signed int int32;
	typedef signed short int16;
	typedef signed char int8;

	typedef int64_pwn uint64;
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
