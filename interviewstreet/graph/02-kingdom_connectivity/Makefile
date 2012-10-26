CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: kingdom_connectivity

kingdom_connectivity: kingdom_connectivity.o

run:
	@./kingdom_connectivity < input00.txt

clean:
	rm -rf *.o *~
