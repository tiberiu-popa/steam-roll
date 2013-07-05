CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: maxdiff

maxdiff: maxdiff.o

run: maxdiff
	@./maxdiff

clean:
	rm -rf *.o *~ *.out maxdiff
