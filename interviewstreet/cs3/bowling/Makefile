CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: bowling

bowling: bowling.o

run:
	@./bowling < input00.txt

clean:
	rm -rf *.o *~
