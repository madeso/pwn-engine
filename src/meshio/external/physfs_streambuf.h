#ifndef PHYSFS_STREAMBUF_H
#define PHYSFS_STREAMBUF_H

#include <streambuf>
#include <string>
#include <vector>
#include <ios>

#include <physfs.h>

class PHYSFS_StreamBuf : public std::basic_streambuf<char>
{
public:
  PHYSFS_StreamBuf(const std::string &filename, bool read, bool write,
      bool append, int readbuffersize = 256, int writebuffersize = 256,
      int putbacksize = 16);

      PHYSFS_StreamBuf(const std::string &filename, std::ios::openmode mode,
          int readbuffersize = 256, int writebuffersize = 256, int putbacksize =
              16);

  virtual
  ~PHYSFS_StreamBuf();

protected:
  virtual int_type
  overflow(int_type c);

  virtual int
  sync();

  virtual std::streampos
  seekpos(std::streampos sp, std::ios::openmode which = std::ios::in
      | std::ios::out);
  virtual std::streampos
  seekoff(std::streamoff off, std::ios::seekdir way, std::ios::openmode which =
      std::ios::in | std::ios::out);

  int_type
  underflow();

  int
  sputc(char c);
  std::streamsize
  sputn(const char *s, std::streamsize n);

  int
  snextc();
  int
  sgetc();
  std::streamsize
  sgetn(char *s, std::streamsize n);

private:
  PHYSFS_file *ifile;
  PHYSFS_file *ofile;
  size_t putbacksize;

  std::vector<char> inbuffer;
  std::vector<char> outbuffer;

  int
  dowrite();
  void
  checkcachevalidity(bool write);
  void
  invalidatereadbuffer();
  void
  doseek(PHYSFS_file *file, std::streampos pos);
  std::streampos
  doseek(PHYSFS_file *file, std::ios::seekdir way, std::streampos offset);
  void
  checkrw(const std::string &filename);

  void
      doopen(const std::string &filename, bool read, bool write, bool append,
          int readbuffersize = 256, int writebuffersize = 256, int putbacksize =
              16);

private:
  //no copying
  PHYSFS_StreamBuf(const PHYSFS_StreamBuf &dummy);
};

#endif
