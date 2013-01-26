CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: matrice6

matrice6: matrice6.o

run: matrice6
	@./matrice6

clean:
	rm -rf *.o *~ *.out matrice6
