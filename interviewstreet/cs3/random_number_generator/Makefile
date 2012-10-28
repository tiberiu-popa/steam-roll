CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: random_number_generator

random_number_generator: random_number_generator.o

run:
	@./random_number_generator < input00.txt

clean:
	rm -rf *.o *~
