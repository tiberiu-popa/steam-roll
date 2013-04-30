CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: welcome-codejam

welcome-codejam: welcome-codejam.o

run: welcome-codejam
	@./welcome-codejam

clean:
	rm -rf *.o *~ *.out welcome-codejam
