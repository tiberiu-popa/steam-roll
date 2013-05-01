CC=ghc
HC=ghc
HCFLAGS=-Wall -O2
.PHONY : clean

all: build

build: fair-warning

fair-warning: fair-warning.o

run: fair-warning
	@./fair-warning

clean:
	rm -rf *.o *~ *.hi *.out fair-warning

%.o: %.lhs
	${HC} -c ${HCFLAGS} $< -o $@

%.o: %.hs
	${HC} -c ${HCFLAGS} $< -o $@
