# compiler 
CXX = g++

#flags
CXXFLAGS = -Wall -std=c++17

SRC = reverse_complement.cpp frequency_analysis.cpp model_probability.cpp markov_generator.cpp
EXE = reverse_complement frequency_analysis model_probability markov_generator

all: $(EXE)

reverse_complement: reverse_complement.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

frequency_analysis: frequency_analysis.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

model_probability: model_probability.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

markov_generator: markov_generator.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXE)