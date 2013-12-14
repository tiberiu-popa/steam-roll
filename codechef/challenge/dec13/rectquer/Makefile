CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: rectquer

rectquer: rectquer.o

run: rectquer
	@./rectquer < rectquer.in

clean:
	rm -rf *.o *~ *.out rectquer
