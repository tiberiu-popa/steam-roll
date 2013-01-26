CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: interzis

interzis: interzis.o

run: interzis
	@./interzis

clean:
	rm -rf *.o *~ *.out interzis
