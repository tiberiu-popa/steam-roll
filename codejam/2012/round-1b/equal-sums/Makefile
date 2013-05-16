CC=g++
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: equal-sums

equal-sums: equal-sums.o

run: equal-sums
	@./equal-sums

clean:
	rm -rf *.o *~ *.out equal-sums
