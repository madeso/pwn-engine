/*
 * physfs_ifstream.h
 *
 *  Created on: 23.04.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_IFSTREAM_H_
#define PHYSFS_IFSTREAM_H_

#include <iostream>
#include <string>
#include "physfs_streambuf.h"
#include "physfs_readfunctions.h"

class PHYSFS_ifstream :  public std::istream, public PHYSFS_ReadFunctions
{
public:
  PHYSFS_ifstream();
  PHYSFS_ifstream(const std::string &filename, ios_base::openmode mode =
      ios_base::in);
  PHYSFS_ifstream(const std::string &filename, int readbuffersize,
      int putbacksize);

  ~PHYSFS_ifstream();

  PHYSFS_StreamBuf*
  rdbuf() const;
  bool
  is_open();
  void
  open(const std::string &filename, ios_base::openmode mode = ios_base::in);
  void
  close();

protected:
private:
  // give read functions access to back read function
  inline virtual void Read(char *data, int size)
  {
    read(data, size);
  }
};

#endif /* PHYSFS_IFSTREAM_H_ */
