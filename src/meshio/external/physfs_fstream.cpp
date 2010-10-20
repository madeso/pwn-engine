/*
 * physfs_fstream.cpp
 *
 *  Created on: 25.04.2010
 *      Author: eazrael
 */

#include "physfs_fstream.h"
using namespace std;

PHYSFS_fstream::PHYSFS_fstream() :
  iostream(0)
{
  //no op
}

PHYSFS_fstream::PHYSFS_fstream(const std::string &filename, ios::openmode mode) :
  iostream(new PHYSFS_StreamBuf(filename, mode))
{
  //no op
}

PHYSFS_fstream::PHYSFS_fstream(const std::string &filename, bool read,
    bool write, bool append, int readbuffersize, int writebuffersize,
    int putbacksize) :
  iostream(new PHYSFS_StreamBuf(filename, read, write, append, readbuffersize,
      writebuffersize, putbacksize))
{
  //no nop
}

PHYSFS_fstream::~PHYSFS_fstream()
{
  PHYSFS_StreamBuf *buf = rdbuf();
  if(buf)
    delete buf;
}

bool
PHYSFS_fstream::is_open()
{
  return iostream::rdbuf() != 0;
}

void
PHYSFS_fstream::close()
{
  streambuf *buf = iostream::rdbuf();
  if(!buf)
    return;

  if(buf->pubsync())
    throw string("Cannot sync streambuf during close");
  delete buf;
  iostream::rdbuf((std::streambuf*) 0);
}

void
PHYSFS_fstream::open(const std::string &filename, ios_base::openmode mode)
{
  if(!mode)
    return;

  close();
  PHYSFS_StreamBuf *buf = new PHYSFS_StreamBuf(filename, mode);
  iostream::rdbuf(buf);
}

PHYSFS_StreamBuf*
PHYSFS_fstream::rdbuf() const
{
  return dynamic_cast<PHYSFS_StreamBuf*> (iostream::rdbuf());
}

