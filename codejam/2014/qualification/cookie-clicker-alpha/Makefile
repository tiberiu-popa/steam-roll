CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cookie-clicker-alpha

cookie-clicker-alpha: cookie-clicker-alpha.o

run: cookie-clicker-alpha
	@./cookie-clicker-alpha < cookie-clicker-alpha.in

clean:
	rm -rf *.o *~ *.out cookie-clicker-alpha
