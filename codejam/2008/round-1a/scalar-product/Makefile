CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: scalar-product

scalar-product: scalar-product.o

run: scalar-product
	@./scalar-product

clean:
	rm -rf *.o *~ *.out scalar-product
