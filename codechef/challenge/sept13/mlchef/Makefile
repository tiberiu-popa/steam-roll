CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: mlchef

mlchef: mlchef.o

run: mlchef
	@./mlchef < mlchef.in

clean:
	rm -rf *.o *~ *.out mlchef
