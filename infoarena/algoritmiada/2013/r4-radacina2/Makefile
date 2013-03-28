CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: radacina2

radacina2: radacina2.o

run: radacina2
	@./radacina2

clean:
	rm -rf *.o *~ *.out radacina2
