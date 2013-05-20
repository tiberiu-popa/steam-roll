CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: amseqt

amseqt: amseqt.o

run: amseqt
	@./amseqt < amseqt.in

clean:
	rm -rf *.o *~ *.out amseqt
