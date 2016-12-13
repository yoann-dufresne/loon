
all: loon

loon: loon.o DynamicProgramming.o Coordz.o Solution.o Problem.o
	g++ -std=c++11 -o $@ -g $^

%.o: %.cpp
	g++ -std=c++11 -c $<

clean:
	rm *.o
