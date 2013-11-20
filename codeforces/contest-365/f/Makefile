CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: f

f: f.o

run: f
	@./f < f.in

clean:
	rm -rf *.o *~ *.out f
