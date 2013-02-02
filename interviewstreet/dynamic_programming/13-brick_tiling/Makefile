CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -g
.PHONY : clean

all: build

build: brick_tiling

brick_tiling: brick_tiling.o

run:
	./brick_tiling < input00.txt

clean:
	rm -rf *.o *~
