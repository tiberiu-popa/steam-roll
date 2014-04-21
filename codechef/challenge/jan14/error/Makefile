GHC=ghc
GHCFLAGS=-Wall -O
.PHONY : clean

all: build

build: error

error: error.hs
	ghc --make $(GHCFLAGS) $<

run: error
	@./error < error.in

clean:
	rm -rf *.o *.hi *~ *.out error
