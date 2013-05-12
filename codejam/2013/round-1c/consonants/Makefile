CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: consonants

consonants: consonants.o

run: consonants
	@./consonants

clean:
	rm -rf *.o *~ *.out consonants
