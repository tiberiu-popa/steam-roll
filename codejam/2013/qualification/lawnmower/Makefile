CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lawnmower

lawnmower: lawnmower.o

run: lawnmower
	@./lawnmower

clean:
	rm -rf *.o *~ *.out lawnmower
