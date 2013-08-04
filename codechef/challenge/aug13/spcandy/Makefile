CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: spcandy

spcandy: spcandy.o

run: spcandy
	@./spcandy < spcandy.in

clean:
	rm -rf *.o *~ *.out spcandy
