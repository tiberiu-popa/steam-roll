CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: almost-equal-advanced

almost-equal-advanced: almost-equal-advanced.o

run: almost-equal-advanced
	@./almost-equal-advanced < almost-equal-advanced.in

clean:
	rm -rf *.o *~ *.out almost-equal-advanced
