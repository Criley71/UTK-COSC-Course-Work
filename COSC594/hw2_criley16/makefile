CXX = g++

CXXFLAGS = -Wall -std=c++17 -g

SRC = globalign.cpp localign.cpp egfalign.cpp ls_egfalign.cpp
EXE = globalign localign egfalign ls_egfalign

all: $(EXE)

globalign: globalign.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

localign: localign.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

egfalign: egfalign.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ls_egfalign: ls_egfalign.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXE)