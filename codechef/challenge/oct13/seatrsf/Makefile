CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: seatrsf

seatrsf: seatrsf.o

run: seatrsf
	@./seatrsf < seatrsf.in

clean:
	rm -rf *.o *~ *.out seatrsf
