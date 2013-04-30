CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: watersheds

watersheds: watersheds.o

run: watersheds
	@./watersheds

clean:
	rm -rf *.o *~ *.out watersheds
