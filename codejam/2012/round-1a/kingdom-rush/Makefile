CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: kingdom-rush

kingdom-rush: kingdom-rush.o

run: kingdom-rush
	@./kingdom-rush

clean:
	rm -rf *.o *~ *.out kingdom-rush
