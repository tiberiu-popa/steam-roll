CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: mvc

mvc: mvc.o

run: mvc
	@./mvc

clean:
	rm -rf *.o *~ *.out mvc
