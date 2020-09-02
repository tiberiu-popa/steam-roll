CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: chefwed

chefwed: chefwed.o

run: chefwed
	@./chefwed < chefwed.in

clean:
	rm -rf *.o *~ *.out chefwed
