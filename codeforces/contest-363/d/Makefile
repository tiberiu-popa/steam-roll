CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: d

d: d.o

run: d
	@./d < d.in

clean:
	rm -rf *.o *~ *.out d
