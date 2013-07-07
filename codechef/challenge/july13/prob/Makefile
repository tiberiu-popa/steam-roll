CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: prob

prob: prob.o

run: prob
	@./prob < prob.in

clean:
	rm -rf *.o *~ *.out prob
