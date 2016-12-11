
all: loon

loon: loon.cpp
	g++ -std=c++11 -o loon -g loon.cpp

clean:
	rm loon
