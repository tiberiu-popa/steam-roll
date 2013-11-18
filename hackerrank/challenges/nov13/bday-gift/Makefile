CC=gcc
CFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: bday-gift

bday-gift: bday-gift.o

run: bday-gift
	@./bday-gift < bday-gift.in

clean:
	rm -rf *.o *~ *.out bday-gift
