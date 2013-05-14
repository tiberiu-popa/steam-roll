CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: garbled-email

garbled-email: garbled-email.o

run: garbled-email
	@./garbled-email

clean:
	rm -rf *.o *~ *.out garbled-email
