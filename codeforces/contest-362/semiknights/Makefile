CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: semiknights

semiknights: semiknights.o

run: semiknights
	@./semiknights < semiknights.in

clean:
	rm -rf *.o *~ *.out semiknights
