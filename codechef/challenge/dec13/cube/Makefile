CC=gcc
CFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: cube

cube: cube.o

run: cube
	@./cube < cube.in

clean:
	rm -rf *.o *~ *.out cube
