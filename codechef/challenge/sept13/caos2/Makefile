CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: caos2

caos2: caos2.o

run: caos2
	@./caos2 < caos2.in

clean:
	rm -rf *.o *~ *.out caos2
