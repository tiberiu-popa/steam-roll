CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: falling-diamonds

falling-diamonds: falling-diamonds.o

run: falling-diamonds
	@./falling-diamonds

clean:
	rm -rf *.o *~ *.out falling-diamonds
