CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: fn

fn: fn.o

run: fn
	@./fn < fn.in

clean:
	rm -rf *.o *~ *.out fn
