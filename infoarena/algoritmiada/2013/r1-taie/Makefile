CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: taie

taie: taie.o

run: taie
	@./taie

clean:
	rm -rf *.o *~ *.out taie
