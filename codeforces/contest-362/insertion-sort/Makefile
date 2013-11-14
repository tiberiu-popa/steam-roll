CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: insertion-sort

insertion-sort: insertion-sort.o

run: insertion-sort
	@./insertion-sort < insertion-sort.in

clean:
	rm -rf *.o *~ *.out insertion-sort
