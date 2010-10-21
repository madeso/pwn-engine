/*
 * physfs_fstream.h
 *
 *  Created on: 25.04.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_FSTREAM_H_
#define PHYSFS_FSTREAM_H_

#include <iostream>
#include <string>
#include "physfs_streambuf.h"
#include "physfs_readfunctions.h"
#include "physfs_writefunctions.h"

class PHYSFS_fstream : public std::iostream, public PHYSFS_ReadFunctions, public PHYSFS_WriteFunctions
{
public:
  PHYSFS_fstream();
  PHYSFS_fstream(const std::string &filename, ios_base::openmode mode =
      ios_base::in | ios_base::out);
  PHYSFS_fstream(const std::string &filename, bool read, bool write,
      bool append, int readbuffersize = 256, int writebuffersize = 256,
      int putbacksize = 16);

  ~PHYSFS_fstream();

  PHYSFS_StreamBuf*
  rdbuf() const;
  bool
  is_open();
  void
  open(const std::string &filename, ios_base::openmode mode = ios_base::in | ios_base::out);
  void
  close();

protected:
private:
  inline void Write(char *data, int size)
  {
    write(data, size);
  }

  inline void Read(char *data, int size)
  {
    read(data, size);
  }
};

#endif /* PHYSFS_FSTREAM_H_ */
