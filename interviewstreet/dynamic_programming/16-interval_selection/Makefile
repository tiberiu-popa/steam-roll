CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -g
.PHONY : clean

all: build

build: interval_selection

interval_selection: interval_selection.o

run:
	@./interval_selection < input00.txt

clean:
	rm -rf *.o *~
