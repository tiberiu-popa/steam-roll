CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: npin

npin: npin.o

run: npin
	@./npin < npin.in

clean:
	rm -rf *.o *~ *.out npin
