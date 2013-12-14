CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: alexnumb

alexnumb: alexnumb.o

run: alexnumb
	@./alexnumb < alexnumb.in

clean:
	rm -rf *.o *~ *.out alexnumb
