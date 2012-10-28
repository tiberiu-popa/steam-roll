CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: exchange

exchange: exchange.o

run:
	@./exchange < input00.txt

clean:
	rm -rf *.o *~
