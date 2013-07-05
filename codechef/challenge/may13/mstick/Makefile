CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: mstick

mstick: mstick.o

run: mstick
	@./mstick < mstick.in

clean:
	rm -rf *.o *~ *.out mstick
