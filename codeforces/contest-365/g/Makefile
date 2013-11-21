CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: g

g: g.o

run: g
	@./g < g.in

clean:
	rm -rf *.o *~ *.out g
