CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: metrou

metrou: metrou.o

run: metrou
	@./metrou

clean:
	rm -rf *.o *~ *.out metrou
