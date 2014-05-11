CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: part-elf

part-elf: part-elf.o

run: part-elf
	@./part-elf < part-elf.in

clean:
	rm -rf *.o *~ *.out part-elf
