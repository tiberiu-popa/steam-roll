CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: box-factory

box-factory: box-factory.o

run: box-factory
	@./box-factory

clean:
	rm -rf *.o *~ *.out box-factory
