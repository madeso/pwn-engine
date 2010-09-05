#ifndef PWN_MESHIO_COMPRESS_INTERNAL_H
#define PWN_MESHIO_COMPRESS_INTERNAL_H

#include <pwn/meshio/io.h>
#include <pwn/number.h>

namespace pwn
{
	namespace meshio
	{
		pwn::uint8 CompressArgsToUint8(const Compress& c);
		Compress Uint8ToCompressArgs(pwn::uint8 u);
	}
}

#endif
