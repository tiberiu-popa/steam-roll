CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: c

c: c.o

run: c
	@./c < c.in

clean:
	rm -rf *.o *~ *.out c
