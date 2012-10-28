CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: power_outage

power_outage: power_outage.o

run:
	@./power_outage < input00.txt

clean:
	rm -rf *.o *~
