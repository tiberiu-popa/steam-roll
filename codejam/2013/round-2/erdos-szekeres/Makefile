CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: erdos-szekeres

erdos-szekeres: erdos-szekeres.o

run: erdos-szekeres
	@./erdos-szekeres

clean:
	rm -rf *.o *~ *.out erdos-szekeres
