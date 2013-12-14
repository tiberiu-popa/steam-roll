CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: chode

chode: chode.o

run: chode
	@./chode < chode.in

clean:
	rm -rf *.o *~ *.out chode
