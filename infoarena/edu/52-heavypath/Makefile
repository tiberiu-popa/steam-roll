CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: heavypath

heavypath: heavypath.o

run: heavypath
	@./heavypath

clean:
	rm -rf *.o *~ *.out heavypath
