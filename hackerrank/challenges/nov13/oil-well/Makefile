CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: oil-well

oil-well: oil-well.o

run: oil-well
	@./oil-well < oil-well.in

clean:
	rm -rf *.o *~ *.out oil-well
