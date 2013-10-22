CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: leexams

leexams: leexams.o

run: leexams
	@./leexams < leexams.in

clean:
	rm -rf *.o *~ *.out leexams
