CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: fcbarca

fcbarca: fcbarca.o

run: fcbarca
	@./fcbarca

clean:
	rm -rf *.o *~ *.out fcbarca
