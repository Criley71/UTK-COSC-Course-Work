CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
SHELL=		bash

all:		primes2

primes2:	primes2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f primes2

test:		test-primes2

test-primes2:	primes2
	@echo Testing easy input/output...
	@diff --suppress-common-lines -y <(./primes2 < input) output

