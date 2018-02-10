#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;

// Program to test performance of mmap for reading of files.
// It reads random buffers from file, after initially mapping
// the entire file from virtual memory to physical memory.
// argv[1] : file path to be read.
// argv[2] : number of times to read buffer.
// argv[3] : size of buffer.
int main(int argc, char **argv) {
  srand(time(nullptr));

  int fd = open(argv[1], O_RDONLY);

  if (fd == -1) {
    cout << "Error opening file for reading";
    exit(1);
  }  

  struct stat fileInfo = {0};
  if (fstat(fd, &fileInfo) == -1) {
    cout << "Error getting the file size";
    exit(1);
  }

  //cout << "\n---" << fileInfo.st_size << "------\n"
  //     << "\n---" << fileInfo.st_blksize << "------\n"
  //     << "\n---" << fileInfo.st_blocks << "------\n";
  long long int size = fileInfo.st_size - (fileInfo.st_size % fileInfo.st_blksize);
  char *map = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (map == MAP_FAILED) {
        close(fd);
        cout << "Error mmapping the file: " << strerror(errno);
        exit(1);
    }

  int num_reads = stoi(argv[2]);
  int buffer_size = stoi(argv[3]);
  char *buffer = new char[buffer_size];
  while(num_reads--) {
      int offset = rand() % (size - buffer_size - 1);
      std::copy(map + offset, map + offset + buffer_size - 1, buffer);
  }
  if (munmap(map, size) == -1)
  {
    close(fd);
    cout << "Error un-mapping the file: " << strerror(errno);
    exit(1);
  }
  close(fd);
}