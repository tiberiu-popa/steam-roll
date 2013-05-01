CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: theme-park

theme-park: theme-park.o

run: theme-park
	@./theme-park

clean:
	rm -rf *.o *~ *.out theme-park
