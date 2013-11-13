CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: sherlock-beast

sherlock-beast: sherlock-beast.o

run: sherlock-beast
	@./sherlock-beast < sherlock-beast.in

clean:
	rm -rf *.o *~ *.out sherlock-beast
