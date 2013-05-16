CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: diamond-inheritance

diamond-inheritance: diamond-inheritance.o

run: diamond-inheritance
	@./diamond-inheritance

clean:
	rm -rf *.o *~ *.out diamond-inheritance
