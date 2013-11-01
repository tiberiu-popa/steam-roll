CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: pretnum

pretnum: pretnum.o

run: pretnum
	@./pretnum < pretnum.in

clean:
	rm -rf *.o *~ *.out pretnum
