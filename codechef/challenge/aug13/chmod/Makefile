CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: chmod

chmod: chmod.o

run: chmod
	@./chmod < chmod.in

clean:
	rm -rf *.o *~ *.out chmod
