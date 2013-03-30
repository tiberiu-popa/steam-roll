CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: drum-bugetat

drum-bugetat: drum-bugetat.o

run: drum-bugetat
	@./drum-bugetat

clean:
	rm -rf *.o *~ *.out drum-bugetat
