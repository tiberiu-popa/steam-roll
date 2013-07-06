CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cirkill

cirkill: cirkill.o

run: cirkill
	@./cirkill < cirkill.in

clean:
	rm -rf *.o *~ *.out cirkill
