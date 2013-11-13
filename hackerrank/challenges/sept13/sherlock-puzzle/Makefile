CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: sherlock-puzzle

sherlock-puzzle: sherlock-puzzle.o

run: sherlock-puzzle
	@./sherlock-puzzle < sherlock-puzzle.in

clean:
	rm -rf *.o *~ *.out sherlock-puzzle
