CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ubercool

ubercool: ubercool.o

run: ubercool
	@./ubercool

clean:
	rm -rf *.o *~ *.out ubercool
