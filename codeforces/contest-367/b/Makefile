CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: b

b: b.o

run: b
	@./b < b.in

clean:
	rm -rf *.o *~ *.out b
