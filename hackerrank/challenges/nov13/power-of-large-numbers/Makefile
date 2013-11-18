CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: power-of-large-numbers

power-of-large-numbers: power-of-large-numbers.o

run: power-of-large-numbers
	@./power-of-large-numbers < power-of-large-numbers.in

clean:
	rm -rf *.o *~ *.out power-of-large-numbers
