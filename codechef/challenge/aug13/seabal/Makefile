CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: seabal

seabal: seabal.o

run: seabal
	@./seabal < seabal.in

clean:
	rm -rf *.o *~ *.out seabal
