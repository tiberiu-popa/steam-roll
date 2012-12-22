CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: x

x: x.o

run: x
	@./x

clean:
	rm -rf *.o *~ *.out x
