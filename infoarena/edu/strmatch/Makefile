CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: strmatch

strmatch: strmatch.o

run:
	@./strmatch

clean:
	rm -rf *.o *~
