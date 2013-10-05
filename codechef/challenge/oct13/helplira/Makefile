CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: helplira

helplira: helplira.o

run: helplira
	@./helplira < helplira.in

clean:
	rm -rf *.o *~ *.out helplira
