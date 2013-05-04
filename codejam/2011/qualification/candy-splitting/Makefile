CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: candy-splitting

candy-splitting: candy-splitting.o

run: candy-splitting
	@./candy-splitting

clean:
	rm -rf *.o *~ *.out candy-splitting
