CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cntsols

cntsols: cntsols.o

run: cntsols
	@./cntsols < cntsols.in

clean:
	rm -rf *.o *~ *.out cntsols
