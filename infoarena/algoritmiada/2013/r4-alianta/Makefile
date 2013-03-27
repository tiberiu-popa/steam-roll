CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: alianta

alianta: alianta.o

run: alianta
	@./alianta

clean:
	rm -rf *.o *~ *.out alianta
