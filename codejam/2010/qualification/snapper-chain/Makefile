CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: snapper-chain

snapper-chain: snapper-chain.o

run: snapper-chain
	@./snapper-chain

clean:
	rm -rf *.o *~ *.out snapper-chain
