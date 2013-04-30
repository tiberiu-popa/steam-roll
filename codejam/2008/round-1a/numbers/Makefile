CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: numbers

numbers: numbers.o

run: numbers
	@./numbers

clean:
	rm -rf *.o *~ *.out numbers
