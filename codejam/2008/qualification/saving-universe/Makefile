CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: saving-universe

saving-universe: saving-universe.o

run: saving-universe
	@./saving-universe

clean:
	rm -rf *.o *~ *.out saving-universe
