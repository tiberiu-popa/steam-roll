CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lemusic

lemusic: lemusic.o

run: lemusic
	@./lemusic

clean:
	rm -rf *.o *~ *.out lemusic
