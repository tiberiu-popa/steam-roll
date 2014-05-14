CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: magic-trick

magic-trick: magic-trick.o

run: magic-trick
	@./magic-trick < magic-trick.in

clean:
	rm -rf *.o *~ *.out magic-trick
