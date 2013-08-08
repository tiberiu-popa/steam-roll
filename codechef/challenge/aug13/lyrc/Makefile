CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lyrc

lyrc: lyrc.o

run: lyrc
	@./lyrc < lyrc.in

clean:
	rm -rf *.o *~ *.out lyrc
