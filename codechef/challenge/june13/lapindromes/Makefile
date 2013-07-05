CC=gcc
CCFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lapindromes

lapindromes: lapindromes.o

run: lapindromes
	@./lapindromes < lapindromes.in

clean:
	rm -rf *.o *~ *.out lapindromes
