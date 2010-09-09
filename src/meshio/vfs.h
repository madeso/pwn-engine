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

		template<class T>
		static void HandleVector(VirtualFile& vf, const std::vector<T>& vector)
		{
			pwn::uint32 size = vector.size();
			vf.write32(size);
			if( size > 0 )
			{
				vf.write(&vector[0], size * sizeof(T));
			}
		}

		template<class T>
		static void HandleVector(VirtualFile& vf, std::vector<T>& vector)
		{
			pwn::uint32 size = vf.read32();
			vector.resize(size);
			if( size > 0 )
			{
				vf.read(&vector[0], size * sizeof(T));
			}
		}

		template<class T>
		static pwn::uint32 HandleVectorSize(VirtualFile& vf, const std::vector<T>& vector)
		{
			pwn::uint32 size = vector.size();
			vf.write32(size);
			return size;
		}

		template<class T>
		static pwn::uint32 HandleVectorSize(VirtualFile& vf, std::vector<T>& vector)
		{
			pwn::uint32 size = vf.read32();
			vector.resize(size);
			return size;
		}

		template<class Value>
		static std::vector<pwn::uint32> HandleKeys(VirtualFile& vf, const std::map<pwn::uint32, Value>& map)
		{
			pwn::uint32 size = map.size();
			vf.write32(size);
			std::vector<pwn::uint32> ret;
			for(std::map<pwn::uint32, Value>::const_iterator i = map.begin(); i != map.end(); ++i)
			{
				const pwn::uint32 v = i->first;
				vf.write32(v);
				ret.push_back(v);
			}
			return ret;
		}

		template<class V>
		static std::vector<pwn::uint32> HandleKeys(VirtualFile& vf, std::map<pwn::uint32, V>&)
		{
			pwn::uint32 size = vf.read32();
			std::vector<pwn::uint32> ret;
			ret.resize(size);
			for(pwn::uint32 i=0; i<size; ++i)
			{
				ret[i] = vf.read32();
			}
			return ret;
		}
	}
}

#endif
