CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: carti3

carti3: carti3.o

run: carti3
	@./carti3

clean:
	rm -rf *.o *~ *.out carti3
