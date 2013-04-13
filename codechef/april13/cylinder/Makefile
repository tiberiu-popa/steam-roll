CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cylinder

cylinder: cylinder.o

run: cylinder
	@./cylinder

clean:
	rm -rf *.o *~ *.out cylinder
