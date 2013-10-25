CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: missile-defend

missile-defend: missile-defend.o

run: missile-defend
	@./missile-defend < missile-defend.in

clean:
	rm -rf *.o *~ *.out missile-defend
