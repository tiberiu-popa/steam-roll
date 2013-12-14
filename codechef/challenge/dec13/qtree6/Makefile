CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wconversion -Wsign-conversion -O2
.PHONY : clean

all: build

build: qtree6

qtree6: qtree6.o

run: qtree6
	@./qtree6 < qtree6.in

clean:
	rm -rf *.o *~ *.out qtree6
