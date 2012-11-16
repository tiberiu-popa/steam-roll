CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: ternar

ternar: ternar.o

run: ternar
	@./ternar

clean:
	rm -rf *.o *~ *.out ternar
