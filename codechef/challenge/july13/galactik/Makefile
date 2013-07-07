CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: galactik

galactik: galactik.o

run: galactik
	@./galactik < galactik.in

clean:
	rm -rf *.o *~ *.out galactik
