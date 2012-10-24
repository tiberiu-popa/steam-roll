CC=gcc
CFLAGS=-Wall -Wextra -O2
.PHONY : clean

all: build

build: candies

candies: candies.o

run:
	@./candies < input00.txt

clean:
	rm -rf *.o *~
