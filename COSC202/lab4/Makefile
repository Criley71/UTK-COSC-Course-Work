CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
SHELL=		bash

all:		solution

solution:	main.o list.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp


list.o:      list.cpp list.h
	$(CXX) $(CXXFLAGS) -c list.cpp

clean:
	rm -f solution *.o

test:		test-output test-memory

test-output:	solution
	@echo Testing output...
	@diff --suppress-common-lines -y <(./solution < input) output

test-memory:	solution
	@echo Testing memory...
	@[ `valgrind --leak-check=full ./solution < input 2>&1 | grep ERROR | awk '{print $$4}'` = 0 ]
