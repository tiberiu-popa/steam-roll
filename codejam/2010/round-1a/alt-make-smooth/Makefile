CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: make-smooth

make-smooth: make-smooth.o

run: make-smooth
	@./make-smooth

clean:
	rm -rf *.o *~ *.out make-smooth
