CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O0#2
.PHONY : clean

all: build

build: rama

rama: rama.o

run: rama
	@./rama

clean:
	rm -rf *.o *~ *.out rama
