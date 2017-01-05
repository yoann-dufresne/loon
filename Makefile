
all: loon

loon: loon.o DynamicProgramming.o Coordz.o Solution.o Problem.o HillClimbing.o
	g++ -std=c++11 -o $@ -O3 $^

%.o: %.cpp
	g++ -std=c++11 -c $< -O3

clean:
	rm *.o
