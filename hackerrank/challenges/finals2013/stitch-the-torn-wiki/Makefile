CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: stitch-the-torn-wiki

stitch-the-torn-wiki: stitch-the-torn-wiki.o

run: stitch-the-torn-wiki
	@./stitch-the-torn-wiki < stitch-the-torn-wiki.in

clean:
	rm -rf *.o *~ *.out stitch-the-torn-wiki
