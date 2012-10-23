CC=gcc
CFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: submat

strmatch: submat.o

run:
	@./submat

clean:
	rm -rf *.o *~
