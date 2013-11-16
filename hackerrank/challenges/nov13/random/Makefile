CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: random

random: random.o

run: random
	@./random < random.in

clean:
	rm -rf *.o *~ *.out random
