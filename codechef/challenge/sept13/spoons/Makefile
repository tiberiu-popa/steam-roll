CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: spoons

spoons: spoons.o

run: spoons
	@./spoons < spoons.in

clean:
	rm -rf *.o *~ *.out spoons
