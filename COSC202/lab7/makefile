CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
SHELL=		bash

all:		nqueen2.cpp

nqueen2:    nqueen2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f nqueen2

test:		test-8 test-10

test-8:	nqueen2
	@echo Testing board size of 8 input/output...
	@diff --suppress-common-lines -y <(./nqueen2 8) output

test-10: nqueen2
	@echo Testing board size of 10 input/output...
	@diff --suppress-common-lines -y <(./nqueen2 10) output2
