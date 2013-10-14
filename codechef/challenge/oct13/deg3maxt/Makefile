CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: deg3maxt

deg3maxt: deg3maxt.o

run: deg3maxt
	@./deg3maxt < deg3maxt.in

clean:
	rm -rf *.o *~ *.out deg3maxt
