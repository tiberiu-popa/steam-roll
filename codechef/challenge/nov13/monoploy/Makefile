CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: monoploy

monoploy: monoploy.o

run: monoploy
	@./monoploy < monoploy.in

clean:
	rm -rf *.o *~ *.out monoploy
