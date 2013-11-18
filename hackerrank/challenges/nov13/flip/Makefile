CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: flip

flip: flip.o

run: flip
	@./flip < flip.in

clean:
	rm -rf *.o *~ *.out flip
