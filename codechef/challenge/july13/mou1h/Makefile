CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: mou1h

mou1h: mou1h.o

run: mou1h
	@./mou1h < mou1h.in

clean:
	rm -rf *.o *~ *.out mou1h
