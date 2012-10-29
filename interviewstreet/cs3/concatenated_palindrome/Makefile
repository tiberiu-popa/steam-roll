CC=gcc
CFLAGS=-Wall -Wextra -Wsign-conversion -g -O2
.PHONY : clean

all: build

build: concatenated_palindrome

concatenated_palindrome: concatenated_palindrome.o

run:
	@./concatenated_palindrome < input00.txt

clean:
	rm -rf *.o *~
