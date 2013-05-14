CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: speaking-tongues

speaking-tongues: speaking-tongues.o

run: speaking-tongues
	@./speaking-tongues

clean:
	rm -rf *.o *~ *.out speaking-tongues
