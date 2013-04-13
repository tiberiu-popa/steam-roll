CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: tic-tac-toe-tomek

tic-tac-toe-tomek: tic-tac-toe-tomek.o

run: tic-tac-toe-tomek
	@./tic-tac-toe-tomek

clean:
	rm -rf *.o *~ *.out tic-tac-toe-tomek
