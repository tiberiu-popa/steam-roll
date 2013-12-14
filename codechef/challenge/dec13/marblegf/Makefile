CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: marblegf

marblegf: marblegf.o

run: marblegf
	@./marblegf < marblegf.in

clean:
	rm -rf *.o *~ *.out marblegf
