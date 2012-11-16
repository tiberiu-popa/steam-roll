CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: facebook

facebook: facebook.o

run: facebook
	@./facebook

clean:
	rm -rf *.o *~ *.out facebook
