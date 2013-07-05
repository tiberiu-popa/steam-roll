CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: spmatrix

spmatrix: spmatrix.o

run: spmatrix
	@./spmatrix < spmatrix.in

clean:
	rm -rf *.o *~ *.out spmatrix
