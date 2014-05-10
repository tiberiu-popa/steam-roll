CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: new-lottery-game

new-lottery-game: new-lottery-game.o

run: new-lottery-game
	@./new-lottery-game < new-lottery-game.in

clean:
	rm -rf *.o *~ *.out new-lottery-game
