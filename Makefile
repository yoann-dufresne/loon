
all: loon

loon: *.cpp *.h
	g++ -std=c++11 -o loon -g loon.cpp

clean:
	rm loon
