CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cruce

ternar: cruce.o

run: cruce
	@./cruce

clean:
	rm -rf *.o *~ *.out cruce
