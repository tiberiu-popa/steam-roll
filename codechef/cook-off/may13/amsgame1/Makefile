CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: amsgame1

amsgame1: amsgame1.o

run: amsgame1
	@./amsgame1 < amsgame1.in

clean:
	rm -rf *.o *~ *.out amsgame1
