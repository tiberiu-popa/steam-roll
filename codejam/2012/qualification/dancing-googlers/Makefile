CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: dancing-googlers

dancing-googlers: dancing-googlers.o

run: dancing-googlers
	@./dancing-googlers

clean:
	rm -rf *.o *~ *.out dancing-googlers
