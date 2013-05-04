CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: magicka

magicka: magicka.o

run: magicka
	@./magicka

clean:
	rm -rf *.o *~ *.out magicka
