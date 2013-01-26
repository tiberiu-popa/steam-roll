CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ismquery

ismquery: ismquery.o

run: ismquery
	@./ismquery

clean:
	rm -rf *.o *~ *.out ismquery
