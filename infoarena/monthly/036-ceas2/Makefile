CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ceas2

ceas2: ceas2.o

run:
	@./ceas2

clean:
	rm -rf *.o *.out *~ ceas2
