GHC=ghc
GHCFLAGS=-Wall -O
.PHONY : clean

all: build

build: plzlykme

plzlykme: plzlykme.hs
	ghc --make $(GHCFLAGS) $<

run: plzlykme
	@./plzlykme < plzlykme.in

clean:
	rm -rf *.o *.hi *~ *.out plzlykme
