CC=gcc
CFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: chocolate-game

chocolate-game: chocolate-game.o

run: chocolate-game
	@./chocolate-game < chocolate-game.in

clean:
	rm -rf *.o *~ *.out chocolate-game
