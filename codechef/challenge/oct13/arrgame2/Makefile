CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: arrgame2

arrgame2: arrgame2.o

run: arrgame2
	@./arrgame2 < arrgame2.in

clean:
	rm -rf *.o *~ *.out arrgame2
