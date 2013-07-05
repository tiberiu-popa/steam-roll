CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: tree

tree: tree.o

run: tree
	@./tree < tree.in

clean:
	rm -rf *.o *~ *.out tree
