CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: kingcon

kingcon: kingcon.o

run: kingcon
	@./kingcon

clean:
	rm -rf *.o *~ *.out kingcon
