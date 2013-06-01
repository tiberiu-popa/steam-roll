CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ticket-swapping

ticket-swapping: ticket-swapping.o

run: ticket-swapping
	@./ticket-swapping

clean:
	rm -rf *.o *~ *.out ticket-swapping
