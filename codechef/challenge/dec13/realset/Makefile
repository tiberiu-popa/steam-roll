CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: realset

realset: realset.o

run: realset
	@./realset < realset.in

clean:
	rm -rf *.o *~ *.out realset
