CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: pogo

pogo: pogo.o

run: pogo
	@./pogo

clean:
	rm -rf *.o *~ *.out pogo
