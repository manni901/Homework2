all:
	g++ -Wall -std=c++11 -g ifstream-example.cpp -o ifstream-example
	g++ -Wall -std=c++11 -g mmap-example.cpp -o mmap-example

clean:
	rm -f *.o
	rm -f *.out
