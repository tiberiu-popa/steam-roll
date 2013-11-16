CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: utopian-tree

utopian-tree: utopian-tree.o

run: utopian-tree
	@./utopian-tree < utopian-tree.in

clean:
	rm -rf *.o *~ *.out utopian-tree
