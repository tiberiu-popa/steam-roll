CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: levy

levy: levy.o

run: levy
	@./levy

clean:
	rm -rf *.o *~ *.out levy
