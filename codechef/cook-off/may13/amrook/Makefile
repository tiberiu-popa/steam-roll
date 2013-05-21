CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: amrook

amrook: amrook.o

run: amrook
	@./amrook amrook.in

clean:
	rm -rf *.o *~ *.out amrook
