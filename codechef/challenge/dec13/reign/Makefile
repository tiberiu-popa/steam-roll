CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: reign

reign: reign.o

run: reign
	@./reign < reign.in

clean:
	rm -rf *.o *~ *.out reign
