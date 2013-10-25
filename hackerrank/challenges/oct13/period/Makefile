CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: period

period: period.o

run: period
	@./period < period.in

clean:
	rm -rf *.o *~ *.out period
