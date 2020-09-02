CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: skmp

skmp: skmp.o

run: skmp
	@./skmp < skmp.in

clean:
	rm -rf *.o *~ *.out skmp
