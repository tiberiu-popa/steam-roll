CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: alien-language

alien-language: alien-language.o

run: alien-language
	@./alien-language

clean:
	rm -rf *.o *~ *.out alien-language
