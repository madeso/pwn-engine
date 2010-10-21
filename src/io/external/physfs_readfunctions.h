/*
 * physfs_readfunctions.h
 *
 *  Created on: 01.05.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_READFUNCTIONS_H_
#define PHYSFS_READFUNCTIONS_H_

#include <iostream>
#include <boost/cstdint.hpp>

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

  inline boost::int64_t
  Read64BE()
  {
    return ReadBE<boost::int64_t> ();
  }
  inline boost::int64_t
  Read64LE()
  {
    return ReadLE<boost::int64_t> ();
  }
  inline boost::int64_t
  Read64Default()
  {
    return ReadDefault<boost::int64_t> ();
  }
  inline boost::int64_t
  Read64()
  {
    return ReadValue<boost::int64_t> ();
  }

  inline boost::uint64_t
  ReadU64BE()
  {
    return ReadBE<boost::uint64_t> ();
  }
  inline boost::uint64_t
  ReadU64LE()
  {
    return ReadLE<boost::uint64_t> ();
  }
  inline boost::uint64_t
  ReadU64Default()
  {
    return ReadDefault<boost::uint64_t> ();
  }
  inline boost::uint64_t
  ReadU64()
  {
    return ReadValue<boost::uint64_t> ();
  }

  inline boost::int32_t
  Read32BE()
  {
    return ReadBE<boost::int32_t> ();
  }
  inline boost::int32_t
  Read32LE()
  {
    return ReadLE<boost::int32_t> ();
  }
  inline boost::int32_t
  Read32Default()
  {
    return ReadDefault<boost::int32_t> ();
  }
  inline boost::int32_t
  Read32()
  {
    return ReadValue<boost::int32_t> ();
  }

  inline boost::uint32_t
  ReadU32BE()
  {
    return ReadBE<boost::uint32_t> ();
  }
  inline boost::uint32_t
  ReadU32LE()
  {
    return ReadLE<boost::uint32_t> ();
  }
  inline boost::uint32_t
  ReadU32Default()
  {
    return ReadDefault<boost::uint32_t> ();
  }
  inline boost::uint32_t
  ReadU32()
  {
    return ReadValue<boost::uint32_t> ();
  }

  inline boost::int16_t
  Read16BE()
  {
    return ReadBE<boost::int16_t> ();
  }
  inline boost::int16_t
  Read16LE()
  {
    return ReadLE<boost::int16_t> ();
  }
  inline boost::int16_t
  Read16Default()
  {
    return ReadDefault<boost::int16_t> ();
  }
  inline boost::int16_t
  Read16()
  {
    return ReadValue<boost::int16_t> ();
  }

  inline boost::uint16_t
  ReadU16BE()
  {
    return ReadBE<boost::uint16_t> ();
  }
  inline boost::uint16_t
  ReadU16LE()
  {
    return ReadLE<boost::uint16_t> ();
  }
  inline boost::uint16_t
  ReadU16Default()
  {
    return ReadDefault<boost::uint16_t> ();
  }
  inline boost::uint16_t
  ReadU16()
  {
    return ReadValue<boost::uint16_t> ();
  }

};

#endif /* PHYSFS_READFUNCTIONS_H_ */
