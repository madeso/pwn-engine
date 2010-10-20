#include <streambuf>
#include <string>
#include <vector>
#include <cstring>
#include "physfs_streambuf.h"

#include <physfs.h>

using namespace std;

PHYSFS_StreamBuf::PHYSFS_StreamBuf(const std::string &filename, bool read,
    bool write, bool append, int readbuffersize, int writebuffersize,
    int putbacksize) :
  ifile(0), ofile(0), putbacksize(putbacksize)
{
  doopen(filename, read, write, append, readbuffersize, writebuffersize,
      putbacksize);
}

PHYSFS_StreamBuf::PHYSFS_StreamBuf(const string &filename, ios::openmode mode,
    int readbuffersize, int writebuffersize, int putbacksize) :
  ifile(0), ofile(0), putbacksize(putbacksize)
{
  bool read = mode & ios::in;
  bool write = mode & ios::out;
  bool append = !(mode & ios::trunc);
  doopen(filename, read, write, append);
  if (mode & ios::ate)
    {
      if (ifile)
        doseek(ifile, ios::end, 0);
      if (ofile)
        doseek(ofile, ios::end, 0);
    }
}

PHYSFS_StreamBuf::~PHYSFS_StreamBuf()
{
  if (!ifile && !ofile)
    return;
  sync();
  if (ifile)
    PHYSFS_close(ifile); //TODO what to do if fails?
  if (ofile)
    PHYSFS_close(ofile); //TODO what to do if fails?
}

void
PHYSFS_StreamBuf::doopen(const std::string &filename, bool read, bool write,
    bool append, int readbuffersize, int writebuffersize, int putbacksize)
{
  if (!read && !write)
    throw string("stop fooling around...");
  if (read && write)
    checkrw(filename);
  if (write)
    {
      outbuffer.reserve(writebuffersize + 1);
      if (append)
        ofile = PHYSFS_openAppend(filename.c_str());
      else
        ofile = PHYSFS_openWrite(filename.c_str());
      if (!ofile)
        throw string("Cannot open file for writing");
      setp(&outbuffer.front(), &outbuffer.front() + outbuffer.capacity() - 1);
    }
  if (read)
    {
      inbuffer.reserve(readbuffersize + putbacksize);
      ifile = PHYSFS_openRead(filename.c_str());
      if (!ifile)
        {
          if (ofile)
            PHYSFS_close(ofile); //TODO what to do if fails???
          throw string("Cannot open file for reading");
        }
      char *endptr = &inbuffer.front() + inbuffer.capacity();
      setg(endptr, endptr, endptr);
    }
}

PHYSFS_StreamBuf::int_type
PHYSFS_StreamBuf::overflow(PHYSFS_StreamBuf::int_type c)
{
  if (c == traits_type::eof())
    return c;
  PHYSFS_sint64 count = pptr() - pbase();
  outbuffer[count] = c;
  pbump(1);
  return dowrite();
}

int
PHYSFS_StreamBuf::sync()
{
  dowrite();
  return 0;
}

int
PHYSFS_StreamBuf::dowrite()
{
  if (!ofile)
    return 0;
  int count = pptr() - pbase();
  if (count == 0)
    return 1;
  PHYSFS_sint64 rc = PHYSFS_write(ofile, pbase(), sizeof(char), count);
  if (rc < 0)
    throw string("Error while writing to file: ") + PHYSFS_getLastError();
  if (rc == 0)
    return 0;
  if (rc == count)
    {
      pbump(-count);
      return count;
    }
  else
    {
      count -= rc;
      for (int i = 0; i < count; ++i)
        outbuffer[i] = outbuffer[rc + i];
      pbump(-count + rc);
      return rc;
    }
}

streampos
PHYSFS_StreamBuf::seekoff(streamoff offset, ios::seekdir way,
    ios::openmode which)
{
  streampos rc = -1;
  if ((which == ios::out) && ofile)
    {
      dowrite();
      checkcachevalidity(true);
      rc = doseek(ofile, way, offset);
    }
  if ((which == ios::in) && ifile)
    {
      dowrite();
      checkcachevalidity(false);
      invalidatereadbuffer();
      rc = doseek(ifile, way, offset);
    }
  return rc;
}

streampos
PHYSFS_StreamBuf::seekpos(std::streampos pos, std::ios::openmode which)
{
  return seekoff(pos, ios::beg, which);
}

PHYSFS_StreamBuf::int_type
PHYSFS_StreamBuf::underflow()
{
  if (gptr() < egptr())
    return *gptr();

  dowrite();
  char *base = &inbuffer.front();
  char *start = base;

  if (eback() == &inbuffer.front())
    {
      memmove(base, egptr() - putbacksize, putbacksize);
      start += putbacksize;
    }
  PHYSFS_sint64 n = PHYSFS_read(ifile, start, 1, base - start
      + inbuffer.capacity());
  if (n == 0)
    return traits_type::eof();

  setg(base, start, start + n);

  return *gptr();
}

int
PHYSFS_StreamBuf::sputc(char c)
{
  checkcachevalidity(true);
  return streambuf::sputc(c);
}

streamsize
PHYSFS_StreamBuf::sputn(const char *s, streamsize n)
{
  checkcachevalidity(true);
  return streambuf::sputn(s, n);
}

inline void
PHYSFS_StreamBuf::checkcachevalidity(bool write)
{
  if (!(ifile && ofile))
    return; //only opened for read or write
  if (write)
    invalidatereadbuffer();
  else
    dowrite();
}

int
PHYSFS_StreamBuf::sgetc()
{
  checkcachevalidity(false);
  return streambuf::sgetc();
}

std::streamsize
PHYSFS_StreamBuf::sgetn(char *s, std::streamsize n)
{
  checkcachevalidity(false);
  return streambuf::sgetn(s, n);
}

int
PHYSFS_StreamBuf::snextc()
{
  checkcachevalidity(false);
  return streambuf::snextc();
}

streampos
PHYSFS_StreamBuf::doseek(PHYSFS_file *file, ios::seekdir way, streampos offset)
{
  PHYSFS_sint64 currpos = PHYSFS_tell(file);
  if (currpos == -1)
    throw string("Cannot perform tell: ") + PHYSFS_getLastError();
  PHYSFS_sint64 size = PHYSFS_fileLength(ofile);
  if (size == -1)
    throw string("Cannot determine file size: ") + PHYSFS_getLastError();

  streampos targetpos = 0;
  if (way == ios::beg)
    targetpos = offset;
  else if (way == ios::cur)
    targetpos = currpos + offset;
  else
    targetpos = size + offset;
  if ((targetpos < 0) || (targetpos > size))
    return -1;
  if (!PHYSFS_seek(file, targetpos))
    return -1;
  else
    return targetpos;
}

void
PHYSFS_StreamBuf::invalidatereadbuffer()
{
  //need to check/kill read buffer
  int filllevel = egptr() - gptr();
  if (!filllevel)
    return;
  if (doseek(ifile, ios::cur, -filllevel) == std::streampos(-1))
    throw string("Seek error while trying to kill read buffer");
  char *endptr = &inbuffer.front() + inbuffer.capacity();
  setg(endptr, endptr, endptr);
}

void
PHYSFS_StreamBuf::checkrw(const string &filename)
{
  if (!PHYSFS_exists(filename.c_str()))
    return;

  PHYSFS_Stat stat;
  int rc = PHYSFS_stat(filename.c_str(), &stat);
  if (!rc)
    throw(string("Cannot stat file before opening: ") + PHYSFS_getLastError());
  if (stat.readonly)
    throw string(
        "File is read-only or write file would be different from the read file");
}
