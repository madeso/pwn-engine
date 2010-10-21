/*
 * main.cpp
 *
 *  Created on: 23.03.2010
 *      Author: eazrael
 */

#include <ostream>
#include <iostream>
#include <sstream>
#include <physfs.h>
#include <unistd.h>
#include <string>

#include "physfs_streambuf.h"
#include "physfs_ifstream.h"
#include "physfs_ofstream.h"
#include "physfs_fstream.h"
using namespace std;

void testwrite()
{
  PHYSFS_ofstream out("test.txt", false, 256);
  out << "Hallo Welt" << endl;
  out.flush();
  ostringstream oss;
  while (oss.str().length() < 1024)
    oss << oss.str().length() << endl;
  out << oss.str();
  out.seekp(0, ios::beg);
  for (int i = 0; i < 1024; i += 2)
    {
      out << "#";
      out.flush();
      out.seekp(i, ios::beg);
    }
  out.seekp(0, ios::beg);
  for (int i = 0; i < 512; i += 4)
    {
      out << "$";
      out.flush();
      out.seekp(4, ios::cur);
    }
  out.seekp(0, ios::end);
  out << endl << "finished" << endl;
  out.flush();
  cout << "write finished" << endl;
}

void testread()
{
  PHYSFS_ifstream in("test.txt");
  string line;
  while(1)
    {
      getline(in, line);
      if(in.eof())
        break;
      cout << line << endl;
    }
  cout << "read finished" << endl;
}

void testrw()
{
  unlink("test.txt");
  cout << "test rw" << endl;
  PHYSFS_fstream io("test.txt", true, true, false);

  io.exceptions(ios::eofbit|ios::failbit|ios::badbit);
  for(int i = 0; i < 40; i++)
    for(char c = 'A'; c <= 'Z'; c++)
      io << c;
  io.seekg(10, ios::beg);
  for(int i = 0; i < 10; i++)
  {
    char c;
    io >> c;
    cout << c;
  }
  io.seekp(10, ios::beg);
  io.seekg(10, ios::beg);
  cout << endl << "Read character a pos 11, write '?' at 14,17,20,23 and read from 12 to 26 " << endl;
  char c;
  io >> c;
  cout << c << endl;
  for(int i = 0; i < 4; i++)
    {
      io.seekp(13 + 3 * i, ios::beg);
      io << '?';
    }
  string line;
  getline(io, line, 'A');
  cout << line << endl;
  cout << "Seek both pointer to start, write 10 times '*' and read and print the first 10 chars" << endl;
  io.seekp(0, ios::beg);
  io.seekg(0, ios::beg);
  for(int i = 0; i < 10; i++)
    io << '*';
  for(int i = 0; i < 10; i++)
    {
      io >> c;
      cout << c;
    }
  cout << endl;
  cout << "test rw finished" << endl;
  unlink("test.txt");
}

void testendianess()
{
	uint64_t varu64;
	uint32_t varu32;
	uint16_t varu16;

	unlink("test.txt");

	PHYSFS_fstream io("test.txt");
	cout << "Write 3 values in system byte order" << endl;
 	io.WriteU16(123);
	io.WriteU32(0x00010000);
	io.WriteU64(0x100000000uLL);

	varu16 = io.ReadU16();
	varu32 = io.ReadU32();
	varu64 = io.ReadU64();

	cout << "Read these 3 values in system byte order" << endl;
	cout << varu16 << " " << varu32 << " " << varu64 << endl;


	cout << "Read these 3 values LE" << endl;
	io.seekg(0, ios::beg);
	varu16 = io.ReadU16LE();
	varu32 = io.ReadU32LE();
	varu64 = io.ReadU64LE();
	cout << varu16 << " " << varu32 << " " << varu64 << endl;


	cout << "Read these 3 values BE" << endl;
	io.seekg(0, ios::beg);
	varu16 = io.ReadU16BE();
	varu32 = io.ReadU32BE();
	varu64 = io.ReadU64BE();
	cout << varu16 << " " << varu32 << " " << varu64 << endl;


	io.close();

	unlink("test.txt");
}



int
main(int argc, char **argv)
{
  PHYSFS_init(argv[0]);
  PHYSFS_addToSearchPath(".", 1);
  PHYSFS_setWriteDir(".");
  try
    {
    testwrite();
    testread();
    testrw();
    testendianess();
    }
  catch (std::exception &e)
    {
      cout << "Exception: " << e.what() << endl;
    }
  catch (string &e)
    {
      cout << "Exception: " << e << endl;
    }
  return 0;
}
