/*
 * physfs_writefunctions.h
 *
 *  Created on: 02.05.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_WRITEFUNCTIONS_H_
#define PHYSFS_WRITEFUNCTIONS_H_

#include <iostream>
#include <boost/cstdint.hpp>

#include "physfs_byteorder.h"

class PHYSFS_WriteFunctions : public virtual PHYSFS_ByteOrder
{
protected:
  virtual void
  Write(char *data, int size) = 0;

public:
  inline
  PHYSFS_WriteFunctions() :
    PHYSFS_ByteOrder()
  {
    //no op
  }

  virtual
  ~PHYSFS_WriteFunctions()
  {
    //no op
  }

  template<typename T>
    inline void
    WriteBE(const T &value)
    {
      T tmp = bigendian ? value : Swap(value);
      Write((char*) &tmp, sizeof(T));
    }

  template<typename T>
    inline void
    WriteLE(const T &value)
    {
      T tmp = bigendian ? Swap(value) : value;
      Write((char*) &tmp, sizeof(T));
    }

  template<typename T>
    inline void
    WriteDefault(const T &value)
    {
      T tmp = defaultconvert ? Swap(value) : value;
      Write((char*) &tmp, sizeof(T));
    }

  template<typename T>
    inline void
    WriteValue(const T &value)
    {
      Write((char*) &value, sizeof(T));
    }

  inline void
  Write64BE(boost::int64_t value)
  {
    WriteBE<boost::int64_t> (value);
  }
  inline void
  Write64LE(boost::int64_t value)
  {
    WriteLE<boost::int64_t> (value);
  }
  inline void
  Write64Default(boost::int64_t value)
  {
    WriteDefault<boost::int64_t> (value);
  }
  inline void
  Write64(boost::int64_t value)
  {
    WriteValue<boost::int64_t> (value);
  }

  inline void
  WriteU64BE(boost::uint64_t value)
  {
    WriteBE<boost::uint64_t> (value);
  }
  inline void
  WriteU64LE(boost::uint64_t value)
  {
    WriteLE<boost::uint64_t> (value);
  }
  inline void
  WriteU64Default(boost::uint64_t value)
  {
    WriteDefault<boost::uint64_t> (value);
  }
  inline void
  WriteU64(boost::uint64_t value)
  {
    WriteValue<boost::uint64_t> (value);
  }

  inline void
  Write32BE(boost::int32_t value)
  {
    WriteBE<boost::int32_t> (value);
  }
  inline void
  Write32LE(boost::int32_t value)
  {
    WriteLE<boost::int32_t> (value);
  }
  inline void
  Write32Default(boost::int32_t value)
  {
    WriteDefault<boost::int32_t> (value);
  }
  inline void
  Write32(boost::int32_t value)
  {
    WriteValue<boost::int32_t> (value);
  }

  inline void
  WriteU32BE(boost::uint32_t value)
  {
    WriteBE<boost::uint32_t> (value);
  }
  inline void
  WriteU32LE(boost::uint32_t value)
  {
    WriteLE<boost::uint32_t> (value);
  }
  inline void
  WriteU32Default(boost::uint32_t value)
  {
    WriteDefault<boost::uint32_t> (value);
  }
  inline void
  WriteU32(boost::uint32_t value)
  {
    WriteValue<boost::uint32_t> (value);
  }

  inline void
  Write16BE(boost::int16_t value)
  {
    WriteBE<boost::int16_t> (value);
  }
  inline void
  Write16LE(boost::int16_t value)
  {
    WriteLE<boost::int16_t> (value);
  }
  inline void
  Write16Default(boost::int16_t value)
  {
    WriteDefault<boost::int16_t> (value);
  }
  inline void
  Write16(boost::int16_t value)
  {
    WriteValue<boost::int16_t> (value);
  }

  inline void
  WriteU16BE(boost::uint16_t value)
  {
    WriteBE<boost::uint16_t> (value);
  }
  inline void
  WriteU16LE(boost::uint16_t value)
  {
    WriteLE<boost::uint16_t> (value);
  }
  inline void
  WriteU16Default(boost::uint16_t value)
  {
    WriteDefault<boost::uint16_t> (value);
  }
  inline void
  WriteU16(boost::uint16_t value)
  {
    WriteValue<boost::uint16_t> (value);
  }
};

#endif /* PHYSFS_WRITEFUNCTIONS_H_ */
