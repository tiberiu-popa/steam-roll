CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: angry-children

angry-children: angry-children.o

run: angry-children
	@./angry-children < angry-children.in

clean:
	rm -rf *.o *~ *.out angry-children
