CC=g++
CXX=g++
CXXFLAGS=-std=c++0x -Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: milkshakes

milkshakes: milkshakes.o

run: milkshakes
	@./milkshakes

clean:
	rm -rf *.o *~ *.out milkshakes
