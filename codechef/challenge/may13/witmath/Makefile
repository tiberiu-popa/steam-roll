CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: Main.class

Main.class: Main.java
	javac $<

run: Main.class
	@java Main < witmath.in

clean:
	rm -rf *.class *~ *.out
