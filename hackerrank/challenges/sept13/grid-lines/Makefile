CC=gcc
CFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: grid-lines

grid-lines: grid-lines.o

run: grid-lines
	@./grid-lines < grid-lines.in

clean:
	rm -rf *.o *~ *.out grid-lines
