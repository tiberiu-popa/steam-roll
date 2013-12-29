CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: best-sum

best-sum: best-sum.o

run: best-sum
	@./best-sum < best-sum.in

clean:
	rm -rf *.o *~ *.out best-sum
