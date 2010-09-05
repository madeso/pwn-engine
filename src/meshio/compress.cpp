#include "compress-internal.h"

namespace pwn
{
	namespace meshio
	{
		Compress::Compress(bool all)
			: materials(all)
			, positions(all)
			, normals(all)
			, texcoords(all)
		{
		}

		namespace // local
		{
			namespace offsets
			{
				enum 
				{
					materials,
					positions,
					normals,
					texcoords
				};
			}
		}

		pwn::uint8 CompressArgsToUint8(const Compress& c)
		{
			uint8 res = 0;
#define WRITE(n) res |= (c.n?1:0) << offsets::n
			WRITE(materials);
			WRITE(positions);
			WRITE(normals);
			WRITE(texcoords);
#undef WRITE
			return res;
		}

		Compress Uint8ToCompressArgs(pwn::uint8 u)
		{
			Compress c(false);
#define READ(n) c.n = (u&(1<<offsets::n)) != 0
			READ(materials);
			READ(positions);
			READ(normals);
			READ(texcoords);
#undef READ
			return c;
		}
	}
}