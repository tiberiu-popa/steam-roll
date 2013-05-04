CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: osmos

osmos: osmos.o

run: osmos
	@./osmos

clean:
	rm -rf *.o *~ *.out osmos
