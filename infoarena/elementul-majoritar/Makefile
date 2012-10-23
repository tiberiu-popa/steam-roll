CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: elmaj

elmaj: elmaj.o

run:
	@./elmaj

clean:
	rm -rf *.o *~
