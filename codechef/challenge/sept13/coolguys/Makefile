CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: coolguys

coolguys: coolguys.o

run: coolguys
	@./coolguys < coolguys.in

clean:
	rm -rf *.o *~ *.out coolguys
