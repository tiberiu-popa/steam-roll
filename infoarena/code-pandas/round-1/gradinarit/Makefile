CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: gradinarit

gradinarit: gradinarit.o

run: gradinarit
	@./gradinarit

clean:
	rm -rf *.o *~ *.out gradinarit
