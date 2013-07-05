CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: name1

name1: name1.o

run: name1
	@./name1 < name1.in

clean:
	rm -rf *.o *~ *.out name1
