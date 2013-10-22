CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: integ

integ: integ.o

run: integ
	@./integ < integ.in

clean:
	rm -rf *.o *~ *.out integ
