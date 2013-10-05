CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: maandi

maandi: maandi.o

run: maandi
	@./maandi < maandi.in

clean:
	rm -rf *.o *~ *.out maandi
