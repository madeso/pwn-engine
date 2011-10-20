#include "vfs.hpp"
#include <pwn/io/io.h>
#include <boost/scoped_array.hpp>
#include <pwn/core/stringutils.h>
#include <pwn/core/str.h>
#include <pwn/core/stringseperator.h>

namespace pwn
{
	namespace io
	{
		namespace
		{
			void Error(const pwn::string& message)
			{
				const char* error = PHYSFS_getLastError();
				const pwn::string vfsError = error ? error : "<null from physfs>";
				throw "Unable to " + message + ": " + vfsError;
			}
		}

		WriteTarget::WriteTarget(const pwn::string& argv0, const pwn::string& target)
		{
			if(0 == PHYSFS_init(argv0.c_str()))
			{
				Error("init");
			}
			if(0 == PHYSFS_addToSearchPath(target.c_str(), 0))
			{
				Error("adding to searchpath: " + target);
			}
			set(target);
		}

		WriteTarget::~WriteTarget()
		{
			PHYSFS_deinit();
		}

		void WriteTarget::set(const pwn::string& target)
		{
			if(0 == PHYSFS_setWriteDir(target.c_str()))
			{
				Error("change write dir to " + target);
			}
		}


		VirtualFile::VirtualFile(const pwn::string& path, bool isLoading)
			: file(isLoading ? PHYSFS_openRead(path.c_str()) : PHYSFS_openWrite(path.c_str()))
		{
			if(file != 0)
			{
				return;
			}
			const pwn::string action = isLoading ? "read" : "write";
			Error(action + " " + path + " can see " + core::StringSeperator().array().iterate(GetFileListing("")).toString());
		}

		VirtualFile::~VirtualFile()
		{
			if(file)
			{
				PHYSFS_close(file);
			}
		}

		void VirtualFile::write8(const pwn::uint8& j)
		{
			const pwn::uint16 i = j;
			if(0 == PHYSFS_writeULE16(file, i))
			{
				Error("write uint8");
			}
		}

		pwn::uint8 VirtualFile::read8()
		{
			pwn::uint16 value = 0;
			if(0 == PHYSFS_readULE16(file, &value))
			{
				Error("read uint8");
			}
			return static_cast<pwn::uint8>(value);
		}

		void VirtualFile::write16(const pwn::uint16& value)
		{
			if(0 == PHYSFS_writeULE16(file, value))
			{
				Error("write uint16");
			}
		}

		pwn::uint16 VirtualFile::read16()
		{
			pwn::uint16 value;
			if(0 == PHYSFS_readULE16(file, &value))
			{
				Error("read uint16");
			}
			return value;
		}

		void VirtualFile::write32(const pwn::uint32& value)
		{
			if(0 == PHYSFS_writeULE32(file, value))
			{
				Error("write uint32");
			}
		}

		pwn::uint32 VirtualFile::read32()
		{
			pwn::uint32 value = 0;
			if(0 == PHYSFS_readULE32(file, &value))
			{
				Error("read uint32");
			}
			return value;
		}

		void VirtualFile::writeReal(const pwn::real& value)
		{
			const PHYSFS_sint64 objects = PHYSFS_write(file, &value, 1, sizeof(pwn::real));
			if(objects != sizeof(pwn::real))
			{
				Error("write real");
			}
		}

		pwn::real VirtualFile::readReal()
		{
			pwn::real value;
			if(PHYSFS_read(file, &value, 1, sizeof(pwn::real)) != sizeof(pwn::real))
			{
				Error("reading real");
			}
			return value;
		}

		void VirtualFile::writeString(const pwn::string& value)
		{
			const uint32 length = value.length();
			write32(length);
			if(PHYSFS_write(file, value.c_str(), 1, length * sizeof(pwn::tchar)) != length * sizeof(pwn::tchar))
			{
				Error("write string");
			}
		}

		pwn::string VirtualFile::readString()
		{
			uint32 length = read32();
			if(length == 0)
			{
				return "";
			}
			else
			{
				boost::scoped_array<pwn::tchar> arr(new tchar[length + 1]);
				if(PHYSFS_read(file, arr.get(), 1, length * sizeof(pwn::tchar)) != length * sizeof(pwn::tchar))
				{
					Error("reading string");
				}
				arr[length] = 0;
				return arr.get();
			}
		}

		void VirtualFile::read(void* data, pwn::uint32 size)
		{
			if(PHYSFS_read(file, data, 1, size) != size)
			{
				Error("reading data");
			}
		}

		void VirtualFile::write(const void* data, pwn::uint32 size)
		{
			const PHYSFS_sint64 objects = PHYSFS_write(file, data, size, 1);
			if(objects != 1)
			{
				Error("writing data");   // size was 1, bug in nightly physfs
			}
		}

		// ---------------------------------------------------------------------------------------------

		void FileWriter::handle8(const pwn::uint8& value)
		{
			file->write8(value);
		}

		void FileWriter::handle16(const pwn::uint16& value)
		{
			file->write16(value);
		}

		void FileWriter::handle32(const pwn::uint32& value)
		{
			file->write32(value);
		}

		void FileWriter::handleReal(const pwn::real& value)
		{
			file->writeReal(value);
		}

		void FileWriter::handleString(const pwn::string& value)
		{
			file->writeString(value);
		}

		// -----------------------------------------------------------------------------------------

		void FileReader::handle8(pwn::uint8& value)
		{
			value = file->read8();
		}

		void FileReader::handle16(pwn::uint16& value)
		{
			value = file->read16();
		}

		void FileReader::handle32(pwn::uint32& value)
		{
			value = file->read32();
		}

		void FileReader::handleReal(pwn::real& value)
		{
			value = file->readReal();
		}

		void FileReader::handleString(pwn::string& value)
		{
			value = file->readString();
		}

		std::vector<pwn::string> GetFileListing(const pwn::string& dir)
		{
			std::vector<pwn::string> files;

			const pwn::string SEPERATOR = "/";
			const bool shouldAddSeperator = !core::EndsWith(dir, SEPERATOR);
			const pwn::string sep = shouldAddSeperator ? SEPERATOR : "";

			char** rc = PHYSFS_enumerateFiles(dir.c_str());
			for(char** i = rc; *i != NULL; i++)
			{
				files.push_back(core::Str() << dir << sep << *i);
			}
			PHYSFS_freeList(rc);

			return files;
		}
	}
}
