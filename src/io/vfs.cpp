#include "vfs.hpp"
#include <pwn/io/io.h>
#include <boost/scoped_array.hpp>
#include <pwn/core/stringutils.h>
#include <pwn/core/str.h>
#include <pwn/core/StringSeperator.h>

#include <fstream>

namespace pwn
{
namespace io
{
    FileHandle::~FileHandle() {}

    struct ReadFileHandle : public FileHandle
    {
        std::ifstream f;
        explicit ReadFileHandle(const std::string& p) : f(p.c_str(), std::ios::binary)
        {
            if(!f)
            {
                throw (core::Str() << "Failed to read " << p).toString();
            }
        }

        void
        write8(const pwn::uint8& j) override { throw "invalid operation"; }

        template<typename T>
        T readSome()
        {
            T t;
            read(&t, sizeof(T));
            return t;
        }

        pwn::uint8
        read8() override { return readSome<pwn::uint8>(); }

        void
        write16(const pwn::uint16& i) override { throw "invalid operation"; }

        pwn::uint16
        read16() override { return readSome<pwn::uint16>(); }

        void
        writeReal(const pwn::real& r) override { throw "invalid operation"; }

        pwn::real
        readReal() override { return readSome<pwn::real>(); }

        void
        write32(const pwn::uint32& i) override { throw "invalid operation"; }

        pwn::uint32
        read32() override { return readSome<pwn::uint32>(); }

        void
        read(void* data, pwn::uint32 size) override
        {
            if(!f.read(static_cast<char*>(data), size))
            {
                throw "failed to read data";
            }
        }
        
        void
        write(const void* data, pwn::uint32 size) override { throw "invalid operation"; }
    };

    struct WriteFileHandle : public FileHandle
    {
        std::ofstream f;
        explicit WriteFileHandle(const std::string& p) : f(p.c_str(), std::ios::out | std::ios::binary)
        {
            if(!f)
            {
                throw (core::Str() << "Failed to write " << p).toString();
            }
        }

        void
        write8(const pwn::uint8& j) override {}

        pwn::uint8
        read8() override { throw "invalid operation"; return 0; }

        void
        write16(const pwn::uint16& i) override {}

        pwn::uint16
        read16() override { throw "invalid operation"; return 0; }

        void
        writeReal(const pwn::real& r) override {}

        pwn::real
        readReal() override { throw "invalid operation"; return 0; }

        void
        write32(const pwn::uint32& i) override {}

        pwn::uint32
        read32() override { throw "invalid operation"; return 0; }

        void
        read(void* data, pwn::uint32 size) override { throw "invalid operation"; }
        
        void
        write(const void* data, pwn::uint32 size) override {}
    };

    VirtualFile::VirtualFile(const pwn::string& path, bool isLoading)
    {
        if(isLoading)
        {
            file.reset(new ReadFileHandle(path));
        }
        else
        {
            file.reset(new WriteFileHandle(path));
        }
    }

    VirtualFile::~VirtualFile()
    {
    }

    void
    VirtualFile::write8(const pwn::uint8& j)
    {
        file->write8(j);
    }

    pwn::uint8
    VirtualFile::read8()
    {
        return file->read8();
    }

    void
    VirtualFile::write16(const pwn::uint16& value)
    {
        file->write16(value);
    }

    pwn::uint16
    VirtualFile::read16()
    {
        return file->read16();
    }

    void
    VirtualFile::write32(const pwn::uint32& value)
    {
        file->write32(value);
    }

    pwn::uint32
    VirtualFile::read32()
    {
        return file->read32();
    }

    void
    VirtualFile::writeReal(const pwn::real& value)
    {
        file->write(&value, sizeof(pwn::real));
    }

    pwn::real
    VirtualFile::readReal()
    {
        pwn::real value;
        file->read(&value, sizeof(pwn::real));
        return value;
    }

    void
    VirtualFile::writeString(const pwn::string& value)
    {
        const uint32 length = value.length();
        write32(length);
        file->write(value.c_str(), length * sizeof(pwn::tchar));
    }

    pwn::string
    VirtualFile::readString()
    {
        uint32 length = read32();
        if (length == 0)
        {
            return "";
        }
        else
        {
            boost::scoped_array<pwn::tchar> arr(new tchar[length + 1]);
            file->read(arr.get(), length * sizeof(pwn::tchar));
            arr[length] = 0;
            return arr.get();
        }
    }

    void
    VirtualFile::read(void* data, pwn::uint32 size)
    {
        file->read(data, size);
    }

    void
    VirtualFile::write(const void* data, pwn::uint32 size)
    {
        file->write(data, size);
    }

    // ---------------------------------------------------------------------------------------------

    void
    FileWriter::handle8(const pwn::uint8& value)
    {
        file->write8(value);
    }

    void
    FileWriter::handle16(const pwn::uint16& value)
    {
        file->write16(value);
    }

    void
    FileWriter::handle32(const pwn::uint32& value)
    {
        file->write32(value);
    }

    void
    FileWriter::handleReal(const pwn::real& value)
    {
        file->writeReal(value);
    }

    void
    FileWriter::handleString(const pwn::string& value)
    {
        file->writeString(value);
    }

    // -----------------------------------------------------------------------------------------

    void
    FileReader::handle8(pwn::uint8& value)
    {
        value = file->read8();
    }

    void
    FileReader::handle16(pwn::uint16& value)
    {
        value = file->read16();
    }

    void
    FileReader::handle32(pwn::uint32& value)
    {
        value = file->read32();
    }

    void
    FileReader::handleReal(pwn::real& value)
    {
        value = file->readReal();
    }

    void
    FileReader::handleString(pwn::string& value)
    {
        value = file->readString();
    }
}
}
