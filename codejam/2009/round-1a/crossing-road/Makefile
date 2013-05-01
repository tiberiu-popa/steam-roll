CC=g++
CXX=g++
CXXFLAGS=-std=c++0x -Wall -Wextra -Wsign-conversion -g#O2
.PHONY : clean

all: build

build: crossing-road

crossing-road: crossing-road.o

run: crossing-road
	@./crossing-road

clean:
	rm -rf *.o *~ *.out crossing-road
