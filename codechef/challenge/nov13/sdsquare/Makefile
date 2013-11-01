CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: sdsquare

sdsquare: sdsquare.o

run: sdsquare
	@./sdsquare < sdsquare.in

clean:
	rm -rf *.o *~ *.out sdsquare
