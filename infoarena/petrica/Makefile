CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -O2
.PHONY : clean

all: build

build: petrica

petrica: petrica.o

run:
	@./petrica

clean:
	rm -rf *.o *~
