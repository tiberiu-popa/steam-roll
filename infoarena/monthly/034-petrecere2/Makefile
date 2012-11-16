CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: petrecere2

petrecere2: petrecere2.o

run: petrecere2
	@./petrecere2

clean:
	rm -rf *.o *~ *.out petrecere2
