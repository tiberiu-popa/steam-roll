CC=gcc
CFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: unfair_game

unfair_game: unfair_game.o

run:
	@./unfair_game < input00.txt

clean:
	rm -rf *.o *~
