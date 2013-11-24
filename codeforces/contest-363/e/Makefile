CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: e

e: e.o

run: e
	@./e < e.in

clean:
	rm -rf *.o *~ *.out e
