CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: lebamboo

lebamboo: lebamboo.o

run: lebamboo
	@./lebamboo < lebamboo.in

clean:
	rm -rf *.o *~ *.out lebamboo
