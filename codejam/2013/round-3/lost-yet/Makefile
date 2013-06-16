CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lost-yet

lost-yet: lost-yet.o

run: lost-yet
	@./lost-yet

clean:
	rm -rf *.o *~ *.out lost-yet
