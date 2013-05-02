CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: number-game

number-game: number-game.o

run: number-game
	@./number-game

clean:
	rm -rf *.o *~ *.out number-game
