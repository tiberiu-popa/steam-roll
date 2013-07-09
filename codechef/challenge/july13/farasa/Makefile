CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: farasa

farasa: farasa.o

run: farasa
	@./farasa < farasa.in

clean:
	rm -rf *.o *~ *.out farasa
