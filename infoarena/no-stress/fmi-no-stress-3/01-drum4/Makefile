CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: drum4

drum4: drum4.o

run: drum4
	@./drum4

clean:
	rm -rf *.o *~ *.out drum4
