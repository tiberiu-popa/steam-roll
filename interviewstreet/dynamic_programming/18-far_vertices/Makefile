CC=gcc
CFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: far_vertices

far_vertices: far_vertices.o

run:
	@./far_vertices < input00.txt

clean:
	rm -rf *.o *~
