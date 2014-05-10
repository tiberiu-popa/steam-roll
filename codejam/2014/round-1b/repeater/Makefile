CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: repeater

repeater: repeater.o

run: repeater
	@./repeater < repeater.in

clean:
	rm -rf *.o *~ *.out repeater
