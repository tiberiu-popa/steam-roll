CC=g++
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: pseudominion

pseudominion: pseudominion.o

run: pseudominion
	@./pseudominion

clean:
	rm -rf *.o *~ *.out pseudominion
