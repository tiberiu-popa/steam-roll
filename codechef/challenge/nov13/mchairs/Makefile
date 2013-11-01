CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: mchairs

mchairs: mchairs.o

run: mchairs
	@./mchairs < mchairs.in

clean:
	rm -rf *.o *~ *.out mchairs
