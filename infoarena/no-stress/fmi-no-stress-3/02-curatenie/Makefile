CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: curatenie

curatenie: curatenie.o

run: curatenie
	@./curatenie

clean:
	rm -rf *.o *~ *.out curatenie
