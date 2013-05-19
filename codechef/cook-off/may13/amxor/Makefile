CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: amxor

amxor: amxor.o

run: amxor
	@./amxor < amxor.in

clean:
	rm -rf *.o *~ *.out amxor
