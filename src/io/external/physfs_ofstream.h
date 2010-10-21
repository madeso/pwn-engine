/*
 * physfs_ofstream.h
 *
 *  Created on: 25.04.2010
 *      Author: eazrael
 */

#ifndef PHYSFS_OFSTREAM_H_
#define PHYSFS_OFSTREAM_H_

#include <iostream>
#include <string>
#include "physfs_streambuf.h"
#include "physfs_writefunctions.h"

class PHYSFS_ofstream : public std::ostream, public PHYSFS_WriteFunctions
{
public:
  PHYSFS_ofstream();
  PHYSFS_ofstream(const std::string &filename, ios_base::openmode mode =
      ios_base::out);
  PHYSFS_ofstream(const std::string &filename, bool append, int writebuffersize);

  ~PHYSFS_ofstream();

  PHYSFS_StreamBuf*
  rdbuf() const;
  bool
  is_open();
  void
  open(const std::string &filename, ios_base::openmode mode = ios_base::out);
  void
  close();

protected:
private:
  inline void
  Write(char *data, int size)
  {
   write(data, size);
  }

};

#endif /* PHYSFS_OFSTREAM_H_ */
