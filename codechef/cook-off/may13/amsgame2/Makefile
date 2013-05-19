CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: amsgame2

amsgame2: amsgame2.o

run: amsgame2
	@./amsgame2 < amsgame2.in

clean:
	rm -rf *.o *~ *.out amsgame2
