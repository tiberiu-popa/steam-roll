CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: muzica

muzica: muzica.o

run: muzica
	@./muzica

clean:
	rm -rf *.o *~ *.out muzica
