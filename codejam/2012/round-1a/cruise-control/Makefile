CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cruise-control

cruise-control: cruise-control.o

run: cruise-control
	@./cruise-control

clean:
	rm -rf *.o *~ *.out cruise-control
