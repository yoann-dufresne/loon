
all: loon

loon: loon.cpp
	g++ -std=c++11 -o loon -g loon.cpp

coordz.o: coordz.cpp
	g++ -std=c++11 -c coordz.cpp

clean:
	rm loon
