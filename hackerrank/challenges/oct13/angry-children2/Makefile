CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: angry-children2

angry-children2: angry-children2.o

run: angry-children2
	@./angry-children2 < angry-children2.in

clean:
	rm -rf *.o *~ *.out angry-children2
