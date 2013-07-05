CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: junongf

junongf: junongf.o

run: junongf
	@./junongf < junongf.in

clean:
	rm -rf *.o *~ *.out junongf
