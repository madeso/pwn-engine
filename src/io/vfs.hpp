#ifndef PWN_IO_VFS_H
#define PWN_IO_VFS_H

#include <pwn/string.h>
#include <pwn/number.h>
#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include <physfs.h>
#include <vector>
#include <map>

namespace pwn
{
	namespace io
	{
		std::vector<pwn::string> GetFileListing(const pwn::string& dir);

		class VirtualFile
			: boost::noncopyable
		{
		public:
			VirtualFile(const pwn::string& path, bool isLoading);
			~VirtualFile();

			void write8(const pwn::uint8& j);
			pwn::uint8 read8();

			void write16(const pwn::uint16& i);
			pwn::uint16 read16();

			void writeReal(const pwn::real& r);
			pwn::real readReal();

			void writeString(const pwn::string& r);
			pwn::string readString();

			void write32(const pwn::uint32& i);
			pwn::uint32 read32();

			void read(void* data, pwn::uint32 size);
			void write(const void* data, pwn::uint32 size);
		private:
			PHYSFS_file* file;
		};

		class FileWriter
		{
		public:
			VirtualFile* file;

			void handle8(const pwn::uint8& j);
			void handle16(const pwn::uint16& i);
			void handle32(const pwn::uint32& i);
			void handleReal(const pwn::real& r);
			void handleString(const pwn::string& r);

			template<class T>
			void handleVector(const std::vector<T>& vector)
			{
				pwn::uint32 size = vector.size();
				file->write32(size);
				if( size > 0 )
				{
					file->write(&vector[0], size * sizeof(T));
				}
			}

			template<class T>
			void handleArray(const boost::scoped_array<T>& arr, pwn::uint32 size)
			{
				if( size > 0 )
				{
					file->write(arr.get(), size * sizeof(T));
				}
			}

			template<class T>
			pwn::uint32 handleVectorSize(const std::vector<T>& vector)
			{
				pwn::uint32 size = vector.size();
				file->write32(size);
				return size;
			}

			template<class Value>
			std::vector<pwn::uint32> handleKeys(const std::map<pwn::uint32, Value>& map)
			{
				pwn::uint32 size = map.size();
				file->write32(size);
				std::vector<pwn::uint32> ret;
				typedef std::map<pwn::uint32, Value> Map;
				for(typename Map::const_iterator i = map.begin(); i != map.end(); ++i)
				{
					const pwn::uint32 v = i->first;
					file->write32(v);
					ret.push_back(v);
				}
				return ret;
			}
		};

		// --------------------------------------------------------------------------------------------------------

		class FileReader
		{
		public:
			VirtualFile* file;

			void handle8(pwn::uint8& j);
			void handle16(pwn::uint16& i);
			void handle32(pwn::uint32& i);
			void handleReal(pwn::real& r);
			void handleString(pwn::string& r);

			template<class T>
			void handleVector(std::vector<T>& vector)
			{
				pwn::uint32 size = file->read32();
				vector.resize(size);
				if( size > 0 )
				{
					file->read(&vector[0], size * sizeof(T));
				}
			}

			template<class T>
			void handleArray(boost::scoped_array<T>& arr, pwn::uint32 size)
			{
				if( size > 0 )
				{
					arr.reset(new T[size]);
					file->read(arr.get(), size * sizeof(T));
				}
				else
				{
					arr.reset();
				}
			}

			template<class T>
			pwn::uint32 handleVectorSize(std::vector<T>& vector)
			{
				pwn::uint32 size = file->read32();
				vector.resize(size);
				return size;
			}

			template<class V>
			std::vector<pwn::uint32> handleKeys(std::map<pwn::uint32, V>&)
			{
				pwn::uint32 size = file->read32();
				std::vector<pwn::uint32> ret;
				ret.resize(size);
				for(pwn::uint32 i=0; i<size; ++i)
				{
					ret[i] = file->read32();
				}
				return ret;
			}

		};

	}
}

#endif
