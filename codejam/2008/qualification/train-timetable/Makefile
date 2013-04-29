CC=g++
CXX=g++
CXXFLAGS=-Wall -Wextra -Wsign-conversion -O2
.PHONY : clean

all: build

build: train-timetable

train-timetable: train-timetable.o

run: train-timetable
	@./train-timetable

clean:
	rm -rf *.o *~ *.out train-timetable
