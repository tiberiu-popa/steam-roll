CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: great-wall

great-wall: great-wall.o

run: great-wall
	@./great-wall

clean:
	rm -rf *.o *~ *.out great-wall
