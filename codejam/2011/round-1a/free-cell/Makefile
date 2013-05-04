CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: free-cell

free-cell: free-cell.o

run: free-cell
	@./free-cell

clean:
	rm -rf *.o *~ *.out free-cell
