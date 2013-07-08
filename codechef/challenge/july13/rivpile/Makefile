CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: rivpile

rivpile: rivpile.o

run: rivpile
	@./rivpile < rivpile.in

clean:
	rm -rf *.o *~ *.out rivpile
