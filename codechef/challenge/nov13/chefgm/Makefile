CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: chefgm

chefgm: chefgm.o

run: chefgm
	@./chefgm < chefgm.in

clean:
	rm -rf *.o *~ *.out chefgm
