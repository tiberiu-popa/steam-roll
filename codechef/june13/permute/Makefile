CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: permute

permute: permute.o

run: permute
	@./permute < permute.in

clean:
	rm -rf *.o *~ *.out permute
