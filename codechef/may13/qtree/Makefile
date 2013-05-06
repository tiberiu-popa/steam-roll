CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: qtree

qtree: qtree.o

run: qtree
	@./qtree < qtree.in

clean:
	rm -rf *.o *~ *.out qtree
