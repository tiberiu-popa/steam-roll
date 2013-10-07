CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: caos3

caos3: caos3.o

run: caos3
	@./caos3 < caos3.in

clean:
	rm -rf *.o *~ *.out caos3
