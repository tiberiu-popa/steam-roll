CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: rotate

rotate: rotate.o

run: rotate
	@./rotate

clean:
	rm -rf *.o *~ *.out rotate
