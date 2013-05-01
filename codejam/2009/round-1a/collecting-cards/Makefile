CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: collecting-cards

collecting-cards: collecting-cards.o

run: collecting-cards
	@./collecting-cards

clean:
	rm -rf *.o *~ *.out collecting-cards
