CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: balulbobocilor

balulbobocilor: balulbobocilor.o

run: balulbobocilor
	@./balulbobocilor

clean:
	rm -rf *.o *~ *.out balulbobocilor
