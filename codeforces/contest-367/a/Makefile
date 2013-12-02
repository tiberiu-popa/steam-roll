CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: a

a: a.o

run: a
	@./a < a.in

clean:
	rm -rf *.o *~ *.out a
