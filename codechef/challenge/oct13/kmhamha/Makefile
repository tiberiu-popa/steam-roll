CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: kmhamha

kmhamha: kmhamha.o

run: kmhamha
	@./kmhamha < kmhamha.in

clean:
	rm -rf *.o *~ *.out kmhamha
