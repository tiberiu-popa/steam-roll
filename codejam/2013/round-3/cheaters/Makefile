CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cheaters

cheaters: cheaters.o

run: cheaters
	@./cheaters

clean:
	rm -rf *.o *~ *.out cheaters
