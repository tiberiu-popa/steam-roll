CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: hall-mirrors

hall-mirrors: hall-mirrors.o

run: hall-mirrors
	@./hall-mirrors

clean:
	rm -rf *.o *~ *.out hall-mirrors
