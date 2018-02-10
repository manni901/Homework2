#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

// Program to test performance stream based reading of files.
// It reads random buffers from file using seekg.
// argv[1] : file path to be read.
// argv[2] : number of times to read buffer.
// argv[3] : size of buffer.
int main(int argc, char **argv) {
  srand(time(nullptr));

  std::ifstream is (argv[1], std::ifstream::binary);
  is.seekg (0, is.end);
  int length = is.tellg();
  is.seekg (0, is.beg);

  int num_reads = stoi(argv[2]);
  int buffer_size = stoi(argv[3]);
  char *buffer = new char[buffer_size];
  while(num_reads--) {
    int offset = rand() % (length - buffer_size - 1);
    is.seekg(offset, is.beg);
    is.read(buffer, buffer_size);
  }
  is.close();
}