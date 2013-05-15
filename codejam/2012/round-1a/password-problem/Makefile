CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: password-problem

password-problem: password-problem.o

run: password-problem
	@./password-problem

clean:
	rm -rf *.o *~ *.out password-problem
