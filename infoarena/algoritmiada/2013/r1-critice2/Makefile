CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: critice2

critice2: critice2.o

run: critice2
	@./critice2

clean:
	rm -rf *.o *~ *.out critice2
