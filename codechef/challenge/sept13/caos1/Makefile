CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: caos1

caos1: caos1.o

run: caos1
	@./caos1 < caos1.in

clean:
	rm -rf *.o *~ *.out caos1
