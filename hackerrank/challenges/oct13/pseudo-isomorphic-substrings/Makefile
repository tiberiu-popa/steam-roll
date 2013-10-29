CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: pseudo-isomorphic-substrings

pseudo-isomorphic-substrings: pseudo-isomorphic-substrings.o

run: pseudo-isomorphic-substrings
	@./pseudo-isomorphic-substrings < pseudo-isomorphic-substrings.in

clean:
	rm -rf *.o *~ *.out pseudo-isomorphic-substrings
