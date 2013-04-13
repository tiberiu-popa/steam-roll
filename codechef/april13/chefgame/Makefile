CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: chefgame

chefgame: chefgame.o

run: chefgame
	@./chefgame

clean:
	rm -rf *.o *~ *.out chefgame
