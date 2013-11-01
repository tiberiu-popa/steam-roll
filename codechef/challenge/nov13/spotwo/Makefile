CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: spotwo

spotwo: spotwo.o

run: spotwo
	@./spotwo < spotwo.in

clean:
	rm -rf *.o *~ *.out spotwo
