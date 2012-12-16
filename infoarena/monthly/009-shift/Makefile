CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: shift

shift: shift.o

run: shift
	@./shift

clean:
	rm -rf *.o *~ *.out shift
