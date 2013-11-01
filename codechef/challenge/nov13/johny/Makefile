CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: johny

johny: johny.o

run: johny
	@./johny < johny.in

clean:
	rm -rf *.o *~ *.out johny
