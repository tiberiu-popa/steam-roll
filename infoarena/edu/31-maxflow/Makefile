CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -O2
.PHONY : clean

all: build

build: maxflow

maxflow: maxflow.o

run:
	@./maxflow

clean:
	rm -rf *.o *~
