CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: bot-trust

bot-trust: bot-trust.o

run: bot-trust
	@./bot-trust

clean:
	rm -rf *.o *~ *.out bot-trust
