CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: safety-numbers

safety-numbers: safety-numbers.o

run: safety-numbers
	@./safety-numbers

clean:
	rm -rf *.o *~ *.out safety-numbers
