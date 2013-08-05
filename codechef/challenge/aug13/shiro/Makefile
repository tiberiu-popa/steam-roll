CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: shiro

shiro: shiro.o

run: shiro
	@./shiro < shiro.in

clean:
	rm -rf *.o *~ *.out shiro
