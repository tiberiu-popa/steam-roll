CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: qpoint

qpoint: qpoint.o

run: qpoint
	@./qpoint < qpoint.in

clean:
	rm -rf *.o *~ *.out qpoint
