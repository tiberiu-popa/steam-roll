CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: goro-sort

goro-sort: goro-sort.o

run: goro-sort
	@./goro-sort

clean:
	rm -rf *.o *~ *.out goro-sort
