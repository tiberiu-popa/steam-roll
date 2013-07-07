CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: kprime

kprime: kprime.o

run: kprime
	@./kprime < kprime.in

clean:
	rm -rf *.o *~ *.out kprime
