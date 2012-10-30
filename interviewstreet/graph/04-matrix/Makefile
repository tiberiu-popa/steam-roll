CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: matrix

matrix: matrix.o

run:
	@./matrix < input00.txt

clean:
	rm -rf *.o *~
