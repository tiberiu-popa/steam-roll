CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: divisible-numbers

divisible-numbers: divisible-numbers.o

run: divisible-numbers
	@./divisible-numbers < divisible-numbers.in

clean:
	rm -rf *.o *~ *.out divisible-numbers
