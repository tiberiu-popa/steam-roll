CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: good-luck

good-luck: good-luck.o

run: good-luck
	@./good-luck

clean:
	rm -rf *.o *~ *.out good-luck
