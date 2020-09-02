CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: subsfreq

subsfreq: subsfreq.o

run: subsfreq
	@./subsfreq < subsfreq.in

clean:
	rm -rf *.o *~ *.out subsfreq
