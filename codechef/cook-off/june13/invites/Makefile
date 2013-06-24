CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: invites

invites: invites.o

run: invites
	@./invites < invites.in

clean:
	rm -rf *.o *~ *.out invites
