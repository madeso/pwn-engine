/*
 * physfs_readfunctions.h
 *
 *  Created on: 01.05.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_READFUNCTIONS_H_
#define PHYSFS_READFUNCTIONS_H_

#include <iostream>
#include <stdint.h>

#include "physfs_byteorder.h"

class PHYSFS_ReadFunctions : public virtual PHYSFS_ByteOrder
{
protected:
  virtual void
  Read(char *data, int size) = 0;

public:
  PHYSFS_ReadFunctions() :
    PHYSFS_ByteOrder()
  {
    //no op
  }

  virtual
  ~PHYSFS_ReadFunctions()
  {
    //no op
  }

  template<typename T>
    inline T
    ReadBE()
    {
      T value;
      Read((char*) &value, sizeof(T));
      return bigendian ? value : Swap(value);
    }

  template<typename T>
    inline T
    ReadLE()
    {
      T value;
      Read((char*) &value, sizeof(T));
      return bigendian ? Swap(value) : value;
    }

  template<typename T>
    inline T
    ReadDefault()
    {
      T value;
      Read((char*) &value, sizeof(T));
      return defaultconvert ? Swap(value) : value;
    }

  template<typename T>
    inline T
    ReadValue()
    {
      T value;
      Read((char*) &value, sizeof(T));
      return value;
    }

  inline int64_t
  Read64BE()
  {
    return ReadBE<int64_t> ();
  }
  inline int64_t
  Read64LE()
  {
    return ReadLE<int64_t> ();
  }
  inline int64_t
  Read64Default()
  {
    return ReadDefault<int64_t> ();
  }
  inline int64_t
  Read64()
  {
    return ReadValue<int64_t> ();
  }

  inline uint64_t
  ReadU64BE()
  {
    return ReadBE<uint64_t> ();
  }
  inline uint64_t
  ReadU64LE()
  {
    return ReadLE<uint64_t> ();
  }
  inline uint64_t
  ReadU64Default()
  {
    return ReadDefault<uint64_t> ();
  }
  inline uint64_t
  ReadU64()
  {
    return ReadValue<uint64_t> ();
  }

  inline int32_t
  Read32BE()
  {
    return ReadBE<int32_t> ();
  }
  inline int32_t
  Read32LE()
  {
    return ReadLE<int32_t> ();
  }
  inline int32_t
  Read32Default()
  {
    return ReadDefault<int32_t> ();
  }
  inline int32_t
  Read32()
  {
    return ReadValue<int32_t> ();
  }

  inline uint32_t
  ReadU32BE()
  {
    return ReadBE<uint32_t> ();
  }
  inline uint32_t
  ReadU32LE()
  {
    return ReadLE<uint32_t> ();
  }
  inline uint32_t
  ReadU32Default()
  {
    return ReadDefault<uint32_t> ();
  }
  inline uint32_t
  ReadU32()
  {
    return ReadValue<uint32_t> ();
  }

  inline int16_t
  Read16BE()
  {
    return ReadBE<int16_t> ();
  }
  inline int16_t
  Read16LE()
  {
    return ReadLE<int16_t> ();
  }
  inline int16_t
  Read16Default()
  {
    return ReadDefault<int16_t> ();
  }
  inline int16_t
  Read16()
  {
    return ReadValue<int16_t> ();
  }

  inline uint16_t
  ReadU16BE()
  {
    return ReadBE<uint16_t> ();
  }
  inline uint16_t
  ReadU16LE()
  {
    return ReadLE<uint16_t> ();
  }
  inline uint16_t
  ReadU16Default()
  {
    return ReadDefault<uint16_t> ();
  }
  inline uint16_t
  ReadU16()
  {
    return ReadValue<uint16_t> ();
  }

};

#endif /* PHYSFS_READFUNCTIONS_H_ */
