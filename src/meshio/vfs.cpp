#include "vfs.h"
#include <pwn/meshio/io.h>
#include <boost/scoped_array.hpp>

namespace pwn
{
	namespace meshio
	{
		namespace
		{
			void Error(const pwn::string& message)
			{
				const pwn::string vfsError = PHYSFS_getLastError();
				throw "Unable to " + message + ": " + vfsError;
			}
		}

		WriteTarget::WriteTarget(const pwn::string& target)
		{
			if( 0 == PHYSFS_init(NULL) ) Error("init");
			set(target);
		}

		WriteTarget::~WriteTarget()
		{
			PHYSFS_deinit();
		}

		void WriteTarget::set(const pwn::string& target)
		{
			if( 0 == PHYSFS_setWriteDir(target.c_str()) ) Error("change write dir");
		}

		
		VirtualFile::VirtualFile(const pwn::string& path, bool isLoading)
			: file(isLoading?PHYSFS_openRead(path.c_str()):PHYSFS_openWrite(path.c_str()))
		{
			if( file != 0 ) return;
			const pwn::string action = isLoading?"read":"write";
			Error(action + " " + path);
		}

		VirtualFile::~VirtualFile()
		{
			if( file ) PHYSFS_close(file);
		}

		void VirtualFile::handle8(const pwn::uint8& j)
		{
			const pwn::uint16 i = j;
			if( 0 == PHYSFS_writeULE16(file, i) ) Error("write uint8");
		}

		void VirtualFile::handle8(pwn::uint8& j)
		{
			pwn::uint16 i=0;
			if( 0 == PHYSFS_readULE16(file, &i) ) Error("read uint8");
			j = static_cast<pwn::uint8>(i);
		}

		void VirtualFile::handle16(const pwn::uint16& i)
		{
			if( 0 == PHYSFS_writeULE16(file, i) ) Error("write uint16");
		}

		void VirtualFile::handle16(pwn::uint16& i)
		{
			if( 0 == PHYSFS_readULE16(file, &i) ) Error("read uint16");
		}

		void VirtualFile::write32(const pwn::uint32& i)
		{
			if( 0 == PHYSFS_writeULE32(file, i) ) Error("write uint32");
		}

		pwn::uint32 VirtualFile::read32()
		{
			pwn::uint32 i = 0;
			if( 0 == PHYSFS_readULE32(file, &i) ) Error("read uint32");
			return i;
		}

		void VirtualFile::handle32(const pwn::uint32& i)
		{
			write32(i);
		}

		void VirtualFile::handle32(pwn::uint32& i)
		{
			i = read32();
		}

		void VirtualFile::handleReal(const pwn::real& r)
		{
			if( PHYSFS_write(file, &r, 1, sizeof(pwn::real)) != sizeof(pwn::real) ) Error("write real");
		}

		void VirtualFile::handleReal(pwn::real& r)
		{
			if( PHYSFS_read(file, &r, 1, sizeof(pwn::real)) != sizeof(pwn::real) ) Error("reading real");
		}

		void VirtualFile::handleString(const pwn::string& r)
		{
			const uint32 length = r.length();
			write32(length);
			if( PHYSFS_write(file, r.c_str(), 1, length*sizeof(pwn::tchar)) != length*sizeof(pwn::tchar) ) Error("write string");
		}

		void VirtualFile::handleString(pwn::string& r)
		{
			uint32 length = read32();
			if( length == 0 )
			{
				r = "";
			}
			else
			{
				boost::scoped_array<pwn::tchar> arr( new tchar[length+1] );
				if( PHYSFS_read(file, arr.get(), 1, length * sizeof(pwn::tchar)) != length*sizeof(pwn::tchar) ) Error("reading string");
				arr[length] = 0;
				r = arr.get();
			}
		}

		void VirtualFile::read(void* data, pwn::uint32 size)
		{
			if( PHYSFS_read(file, data, size, 1) != 1 ) Error("reading data");
		}

		void VirtualFile::write(const void* data, pwn::uint32 size)
		{
			if( PHYSFS_write(file, data, size, 1) != 1 ) Error("writing data");
		}
	}
}
