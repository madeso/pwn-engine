#include "PhysFS++.h"

int main(int argc, char** argv)
{
  // init PhysFS system
  PhysFS::init(argv[0]);

  // add the program's base directory to the search path
  PhysFS::addToSearchPath(PhysFS::getBaseDir());
  // set the program's base directory as the write dir
  PhysFS::setWriteDir(PhysFS::getBaseDir());

  // create some text file and write to it
  PhysFS::FileStream stream ("test.txt", PhysFS::OM_WRITE);
  stream << "Just a standard stream: " << 12345 << "\n";
  stream.close();

  // shutdown PhysFS
  PhysFS::deinit();
}