CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: tide-inout

tide-inout: tide-inout.o

run: tide-inout
	@./tide-inout

clean:
	rm -rf *.o *~ *.out tide-inout
