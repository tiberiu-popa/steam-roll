CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lumanari

lumanari: lumanari.o

run: lumanari
	@./lumanari

clean:
	rm -rf *.o *~ *.out lumanari
