CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: decorate

decorate: decorate.o

run: decorate
	@./decorate < decorate.in

clean:
	rm -rf *.o *~ *.out decorate
