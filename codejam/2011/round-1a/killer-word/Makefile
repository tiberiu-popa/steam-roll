CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: killer-word

killer-word: killer-word.o

run: killer-word
	@./killer-word

clean:
	rm -rf *.o *~ *.out killer-word
