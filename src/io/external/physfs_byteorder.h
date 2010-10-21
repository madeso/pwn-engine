/*
 * physfs_byteorder.h
 *
 *  Created on: 02.05.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_BYTEORDER_H_
#define PHYSFS_BYTEORDER_H_

class PHYSFS_ByteOrder
{
protected:
  bool bigendian;
  bool defaultconvert;
public:
  inline
  PHYSFS_ByteOrder() :
    defaultconvert(false)
  {
    //check endianness
    union
    {
      char bytes[4];
      boost::uint32_t value;
    } testunion;
    testunion.value = 1;
    bigendian = testunion.bytes[3];
  }

  virtual
  ~PHYSFS_ByteOrder()
  {
    //no op
  }

  inline void
  SetBigEndianDefault()
  {
    defaultconvert = !bigendian;
  }

  inline void
  SetLittleEndianDefault()
  {
    defaultconvert = bigendian;
  }

  inline bool
  IsBigEndianDefault() const
  {
    return bigendian != defaultconvert;
  }

  inline bool
  IsLittleEndianDefault() const
  {
    return bigendian == defaultconvert;
  }

  //abstract function we
  inline boost::uint64_t
  Swap(boost::uint64_t value)
  {
    boost::uint64_t rc = 0;
    boost::uint8_t *src = (boost::uint8_t*) &value;
    boost::uint8_t *tgt = (boost::uint8_t*) &rc;

    tgt[0] = src[7];
    tgt[1] = src[6];
    tgt[2] = src[5];
    tgt[3] = src[4];
    tgt[4] = src[3];
    tgt[5] = src[2];
    tgt[6] = src[1];
    tgt[7] = src[0];

    return rc;
  }

  inline boost::int64_t
  Swap(boost::int64_t value)
  {
    boost::int64_t rc = 0;
    boost::int8_t *src = (boost::int8_t*) &value;
    boost::int8_t *tgt = (boost::int8_t*) &rc;

    tgt[0] = src[7];
    tgt[1] = src[6];
    tgt[2] = src[5];
    tgt[3] = src[4];
    tgt[4] = src[3];
    tgt[5] = src[2];
    tgt[6] = src[1];
    tgt[7] = src[0];

    return rc;
  }

  inline boost::uint32_t
  Swap(boost::uint32_t value)
  {
    boost::uint32_t rc = 0;
    boost::uint8_t *src = (boost::uint8_t*) &value;
    boost::uint8_t *tgt = (boost::uint8_t*) &rc;

    tgt[0] = src[3];
    tgt[1] = src[2];
    tgt[2] = src[1];
    tgt[3] = src[0];

    return rc;
  }

  inline boost::int32_t
  Swap(boost::int32_t value)
  {
    boost::int32_t rc = 0;
    boost::int8_t *src = (boost::int8_t*) &value;
    boost::int8_t *tgt = (boost::int8_t*) &rc;

    tgt[0] = src[3];
    tgt[1] = src[2];
    tgt[2] = src[1];
    tgt[3] = src[0];

    return rc;
  }

  inline boost::uint16_t
  Swap(boost::uint16_t value)
  {
    return ((value << 8) | (value >> 8));
  }

  inline boost::int16_t
  Swap(boost::int16_t value)
  {
    return ((value << 8) | (value >> 8));
  }


};



#endif /* PHYSFS_BYTEORDER_H_ */
