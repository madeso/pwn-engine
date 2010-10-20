/*
 * physfs_writefunctions.h
 *
 *  Created on: 02.05.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_WRITEFUNCTIONS_H_
#define PHYSFS_WRITEFUNCTIONS_H_

#include <iostream>
#include <stdint.h>

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
  Write64BE(int64_t value)
  {
    WriteBE<int64_t> (value);
  }
  inline void
  Write64LE(int64_t value)
  {
    WriteLE<int64_t> (value);
  }
  inline void
  Write64Default(int64_t value)
  {
    WriteDefault<int64_t> (value);
  }
  inline void
  Write64(int64_t value)
  {
    WriteValue<int64_t> (value);
  }

  inline void
  WriteU64BE(uint64_t value)
  {
    WriteBE<uint64_t> (value);
  }
  inline void
  WriteU64LE(uint64_t value)
  {
    WriteLE<uint64_t> (value);
  }
  inline void
  WriteU64Default(uint64_t value)
  {
    WriteDefault<uint64_t> (value);
  }
  inline void
  WriteU64(uint64_t value)
  {
    WriteValue<uint64_t> (value);
  }

  inline void
  Write32BE(int32_t value)
  {
    WriteBE<int32_t> (value);
  }
  inline void
  Write32LE(int32_t value)
  {
    WriteLE<int32_t> (value);
  }
  inline void
  Write32Default(int32_t value)
  {
    WriteDefault<int32_t> (value);
  }
  inline void
  Write32(int32_t value)
  {
    WriteValue<int32_t> (value);
  }

  inline void
  WriteU32BE(uint32_t value)
  {
    WriteBE<uint32_t> (value);
  }
  inline void
  WriteU32LE(uint32_t value)
  {
    WriteLE<uint32_t> (value);
  }
  inline void
  WriteU32Default(uint32_t value)
  {
    WriteDefault<uint32_t> (value);
  }
  inline void
  WriteU32(uint32_t value)
  {
    WriteValue<uint32_t> (value);
  }

  inline void
  Write16BE(int16_t value)
  {
    WriteBE<int16_t> (value);
  }
  inline void
  Write16LE(int16_t value)
  {
    WriteLE<int16_t> (value);
  }
  inline void
  Write16Default(int16_t value)
  {
    WriteDefault<int16_t> (value);
  }
  inline void
  Write16(int16_t value)
  {
    WriteValue<int16_t> (value);
  }

  inline void
  WriteU16BE(uint16_t value)
  {
    WriteBE<uint16_t> (value);
  }
  inline void
  WriteU16LE(uint16_t value)
  {
    WriteLE<uint16_t> (value);
  }
  inline void
  WriteU16Default(uint16_t value)
  {
    WriteDefault<uint16_t> (value);
  }
  inline void
  WriteU16(uint16_t value)
  {
    WriteValue<uint16_t> (value);
  }
};

#endif /* PHYSFS_WRITEFUNCTIONS_H_ */
