CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: recycled-numbers

recycled-numbers: recycled-numbers.o

run: recycled-numbers
	@./recycled-numbers

clean:
	rm -rf *.o *~ *.out recycled-numbers
