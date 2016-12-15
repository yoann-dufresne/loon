
all: loon

loon: loon.o DynamicProgramming.o Coordz.o Solution.o Problem.o
	g++ -std=c++11 -o $@ -g $^ -pg

%.o: %.cpp
	g++ -std=c++11 -c $< -g -pg

clean:
	rm *.o
