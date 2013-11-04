CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: gerald2

gerald2: gerald2.o

run: gerald2
	@./gerald2 < gerald2.in

clean:
	rm -rf *.o *~ *.out gerald2
