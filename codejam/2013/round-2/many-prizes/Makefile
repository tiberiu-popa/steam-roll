CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: many-prizes

many-prizes: many-prizes.o

run: many-prizes
	@./many-prizes

clean:
	rm -rf *.o *~ *.out many-prizes
