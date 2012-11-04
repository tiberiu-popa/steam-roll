CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: liars

liars: liars.o

run: liars
	@./liars < input00.txt

clean:
	rm -rf *.o *~ liars
