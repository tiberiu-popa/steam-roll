CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: chrect

chrect: chrect.o

run: chrect
	@./chrect < chrect.in

clean:
	rm -rf *.o *~ *.out chrect
