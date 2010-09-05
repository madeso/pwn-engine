#ifndef PWN_MESHIO_VFS_H
#define PWN_MESHIO_VFS_H

#include <pwn/string.h>
#include <pwn/number.h>
#include <boost/noncopyable.hpp>

#include <physfs.h>

namespace pwn
{
	namespace meshio
	{
		class VirtualFile
			: boost::noncopyable
		{
		public:
			VirtualFile(const pwn::string& path, bool isLoading);
			~VirtualFile();

			void handle8(const pwn::uint8& j);
			void handle8(pwn::uint8& j);

			void handle16(const pwn::uint16& i);
			void handle16(pwn::uint16& i);

			void handle32(const pwn::uint32& i);
			void handle32(pwn::uint32& i);

			void handleReal(const pwn::real& r);
			void handleReal(pwn::real& r);

			void handleString(const pwn::string& r);
			void handleString(pwn::string& r);

			void write32(const pwn::uint32& i);
			pwn::uint32 read32();

			void read(void* data, pwn::uint32 size);
			void write(const void* data, pwn::uint32 size);

		private:
			PHYSFS_file* file;
		};
	}
}

#endif
