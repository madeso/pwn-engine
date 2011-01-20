#include "vfs_util.hpp"

namespace pwn
{
	namespace engine
	{
		File::File(PHYSFS_file* file)
			: file(file)
		{
			if( file == 0 ) throw "failed to load file";
		}

		File::~File()
		{
			PHYSFS_close(file);
		}

		const std::size_t File::loadToMemory(boost::scoped_array<byte>* memory)
		{
			const PHYSFS_sint64 ssize = PHYSFS_fileLength(file);
			if( ssize < 0 ) throw "failed to load to get length";
			const std::size_t size = ssize;
			memory->reset(new byte[size]);
			const PHYSFS_sint64 read = PHYSFS_read(file, memory->get(), 1, size);
			if( read != ssize ) throw "failed to read data";
			return size;
		}
	}
}