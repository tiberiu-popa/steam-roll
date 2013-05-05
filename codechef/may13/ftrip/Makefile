CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ftrip

ftrip: ftrip.o

run: ftrip
	@./ftrip < ftrip.in

clean:
	rm -rf *.o *~ *.out ftrip
