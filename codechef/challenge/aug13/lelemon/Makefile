CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lelemon

lelemon: lelemon.o

run: lelemon
	@./lelemon < lelemon.in

clean:
	rm -rf *.o *~ *.out lelemon
