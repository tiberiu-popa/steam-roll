CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: multi-happiness

multi-happiness: multi-happiness.o

run: multi-happiness
	@./multi-happiness

clean:
	rm -rf *.o *~ *.out multi-happiness
