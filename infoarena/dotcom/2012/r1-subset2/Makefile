CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: subset2

subset2: subset2.o

run: subset2
	@./subset2

clean:
	rm -rf *.o *~ *.out subset2
