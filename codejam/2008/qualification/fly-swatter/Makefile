CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: fly-swatter

fly-swatter: fly-swatter.o

run: fly-swatter
	@./fly-swatter

clean:
	rm -rf *.o *~ *.out fly-swatter
