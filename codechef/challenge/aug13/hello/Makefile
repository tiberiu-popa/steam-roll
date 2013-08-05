CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: hello

hello: hello.o

run: hello
	@./hello < hello.in

clean:
	rm -rf *.o *~ *.out hello
