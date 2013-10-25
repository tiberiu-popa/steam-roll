CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: chocolate-feast

chocolate-feast: chocolate-feast.o

run: chocolate-feast
	@./chocolate-feast < chocolate-feast.in

clean:
	rm -rf *.o *~ *.out chocolate-feast
