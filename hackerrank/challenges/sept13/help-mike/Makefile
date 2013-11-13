CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: help-mike

help-mike: help-mike.o

run: help-mike
	@./help-mike < help-mike.in

clean:
	rm -rf *.o *~ *.out help-mike
