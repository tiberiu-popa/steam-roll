CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: manage-energy

manage-energy: manage-energy.o

run: manage-energy
	@./manage-energy

clean:
	rm -rf *.o *~ *.out manage-energy
