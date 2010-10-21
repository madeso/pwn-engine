/*
 * physfs_ofstream.cpp
 *
 *  Created on: 25.04.2010
 *      Author: eazrael
 */

#include "physfs_ofstream.h"
using namespace std;

PHYSFS_ofstream::PHYSFS_ofstream() :
  ostream(0)
{
  //no op
}

PHYSFS_ofstream::PHYSFS_ofstream(const std::string &filename,
    ios::openmode mode) :
  ostream(new PHYSFS_StreamBuf(filename, mode
      & (ios::out | ios::app | ios::ate)))
{
  //no op
}

PHYSFS_ofstream::PHYSFS_ofstream(const std::string &filename, bool append,
    int writebuffersize) :
  ostream(new PHYSFS_StreamBuf(filename, false, true, append, 256, writebuffersize, 16))
{
  //no op
}

PHYSFS_ofstream::~PHYSFS_ofstream()
{
  PHYSFS_StreamBuf *buf = rdbuf();
  if(buf)
    delete buf;
}


bool
PHYSFS_ofstream::is_open()
{
  return ostream::rdbuf() != 0;
}

void
PHYSFS_ofstream::close()
{
  streambuf *buf = ostream::rdbuf();
  if(!buf)
    return;

  if(buf->pubsync())
    throw string("Cannot sync streambuf during close");
  delete buf;
  ostream::rdbuf((std::streambuf*) 0);
}

void
PHYSFS_ofstream::open(const std::string &filename, ios_base::openmode mode)
{
  mode &= ios::out | ios::app | ios::ate;
  if(!mode)
    return;

  close();
  PHYSFS_StreamBuf *buf = new PHYSFS_StreamBuf(filename, mode);
 ostream::rdbuf(buf);
}

PHYSFS_StreamBuf*
PHYSFS_ofstream::rdbuf() const
{
  return dynamic_cast<PHYSFS_StreamBuf*> (ostream::rdbuf());
}

