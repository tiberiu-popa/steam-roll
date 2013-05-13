CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: cpp

cpp: cpp.o
	$(CC) $^ $(LDFLAGS) -o $@

run: cpp
	@./cpp < cpp.in

clean:
	rm -rf *.o *~ *.out cpp
