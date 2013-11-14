CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: staircases

staircases: staircases.o

run: staircases
	@./staircases < staircases.in

clean:
	rm -rf *.o *~ *.out staircases
