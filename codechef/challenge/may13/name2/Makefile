CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: name2

name2: name2.o

run: name2
	@./name2 < name2.in

clean:
	rm -rf *.o *~ *.out name2
