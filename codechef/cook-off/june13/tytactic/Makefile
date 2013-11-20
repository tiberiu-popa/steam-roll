CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: tytactic

tytactic: tytactic.o

run: tytactic
	@./tytactic < tytactic.in

clean:
	rm -rf *.o *~ *.out tytactic
