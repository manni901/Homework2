# Testing Performance of stream based file read vs memory-based mapping (mmap)
  In computing, mmap is a POSIX-compliant Unix system call that maps files or devices into memory. It is a method of memory-mapped file I/O. It naturally implements demand paging, because file contents are not read from disk initially and do not use physical RAM at all. The actual reads from disk are performed in a "lazy" manner, after a specific location is accessed. (Wikipedia)
When the reads are non-sequential and file sizes are large, mmap can be extremely fast compared to standard stream based i/o. Some points on when mmap can be benefitial and when its not:
  - A call to mmap has more overhead than read (just like epoll has more overhead than poll, which has more overhead than read). Changing virtual memory mappings is a quite expensive operation on some processors for the same reasons that switching between different processes is expensive.
  - The IO system can already use the disk cache, so if you read a file, you'll hit the cache or miss it no matter what method you use.
  - Memory maps are generally faster for random access, especially if your access patterns are sparse and unpredictable.
  - Memory maps allow you to keep using pages from the cache until you are done. This means that if you use a file heavily for a long period of time, then close it and reopen it, the pages will still be cached. With read, your file may have been flushed from the cache ages ago. This does not apply if you use a file and immediately discard it. (If you try to mlock pages just to keep them in cache, you are trying to outsmart the disk cache and this kind of foolery rarely helps system performance).
  - Reading a file directly is very simple and fast.

  Conclusion: Use memory maps if you access data randomly, keep it around for a long time, or if you know you can share it with other processes (MAP_SHARED isn't very interesting if there is no actual sharing). Read files normally if you access data sequentially or discard it after reading. And if either method makes your program less complex, do that. For many real world cases there's no sure way to show one is faster without testing your actual application and NOT a benchmark. (This summary and all the above points have been taken from a stackoverflow answer https://stackoverflow.com/a/6383253)
  
# Code
  - The code contains two files ifstream-example.cpp and mmap-example.cpp. Both programs read random chunks of input file for a given number of iterations.
  - To build the code run the following on the command line. ```$ make```
  - To run type ```$ time ./ifstream-example [FILE_PATH] [NUM_ITERATIONS] [CHUNK_SIZE]```. Run the same command for mmap-example. 

# Results
I tested the code on two machines, one with SSD and one with a HDD. Following are the results.
```sh
Machine 1: Lenovo G500 Ubuntu 16.04 Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz, 4GB RAM, L1: 32K, L2: 256K, L3: 4M, HDD: SATA 3.0 5400 rpm
File: 680 MB text file
$ time ./ifstream-example large.txt 1000 256
real	0m4.469s
user	0m0.000s
sys	0m0.024s

$ time ./mmap-example large.txt 1000 256
real	0m4.499s
user	0m0.000s
sys	0m0.032s

$ time ./mmap-example large.txt 1000 4096
real	0m4.303s
user	0m0.004s
sys	0m0.024s

$ time ./ifstream-example large.txt 1000 4096
real	0m3.906s
user	0m0.012s
sys	0m0.012s
```

```sh
Machine 2: Apple Macbook Pro14,3 Intel(R) Core(TM) i7 CPU @ 3.1GHz, 16GB RAM, L1: 32K, L2: 256K, L3: 8M, SSD: 512GB PCIe-based onboard SSD
File: 1.4 GB tsv file
$ time ./ifstream-example OH.tsv 1000 256
real	0m0.373s
user	0m0.010s
sys	0m0.088s

$ time ./mmap-example OH.tsv 1000 256
real	0m0.188s
user	0m0.004s
sys	0m0.046s

$ time ./mmap-example OH.tsv 1000 4096
real	0m0.138s
user	0m0.006s
sys	0m0.044s
```

