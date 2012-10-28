CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ncr

ncr: ncr.o

run:
	@./ncr < input00.txt

clean:
	rm -rf *.o *~
