CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: drumarb

drumarb: drumarb.o

run: drumarb
	@./drumarb

clean:
	rm -rf *.o *~ *.out drumarb
