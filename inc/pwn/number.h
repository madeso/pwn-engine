#ifndef PWN_CONFIG
#define PWN_CONFIG

namespace pwn
{
	// flaots
	typedef float float32;

	// integers too...
	typedef __int64 int64; // long long for other than msvc++?
	typedef int int32;
	typedef short int16;
	typedef char int8;

	typedef unsigned __int64 uint64;
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