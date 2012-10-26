CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: problem_solving

problem_solving: problem_solving.o

run:
	@./problem_solving < input00.txt

clean:
	rm -rf *.o *~
