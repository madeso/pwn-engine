/*
 * physfs_ifstream.cpp
 *
 *  Created on: 25.04.2010
 *      Author: eazrael
 */

#include "physfs_ifstream.h"
using namespace std;

PHYSFS_ifstream::PHYSFS_ifstream() :
  istream(0), PHYSFS_ReadFunctions()
{
  //no op
}

PHYSFS_ifstream::PHYSFS_ifstream(const std::string &filename,
    ios::openmode mode) :
  istream(new PHYSFS_StreamBuf(filename, mode & (ios::in | ios::ate))),
      PHYSFS_ReadFunctions()
{
  //no op
}

PHYSFS_ifstream::PHYSFS_ifstream(const std::string &filename,
    int readbuffersize, int putbacksize) :
istream(new PHYSFS_StreamBuf(filename, readbuffersize, 256,
        putbacksize)), PHYSFS_ReadFunctions()
  {
    //no op
  }

PHYSFS_ifstream::~PHYSFS_ifstream()
{
  PHYSFS_StreamBuf *buf = rdbuf();
  if(buf)
    delete buf;
}

bool
PHYSFS_ifstream::is_open()
{
  return istream::rdbuf() != 0;
}

void
PHYSFS_ifstream::close()
{
  streambuf *buf = istream::rdbuf();
  if(!buf)
    return;

  if(buf->pubsync())
    throw string("Cannot sync streambuf during close");
  delete buf;
  istream::rdbuf((std::streambuf*) 0);
}

void
PHYSFS_ifstream::open(const std::string &filename, ios_base::openmode mode)
{
  mode &= ios::in | ios::ate;
  if(!mode)
    return;

  close();
  PHYSFS_StreamBuf *buf = new PHYSFS_StreamBuf(filename, mode);
  istream::rdbuf(buf);
}

PHYSFS_StreamBuf*
PHYSFS_ifstream::rdbuf() const
{
  return dynamic_cast<PHYSFS_StreamBuf*> (istream::rdbuf());
}

