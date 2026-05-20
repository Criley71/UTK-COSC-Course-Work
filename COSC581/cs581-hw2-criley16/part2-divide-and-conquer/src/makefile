CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Wno-sign-compare

SRC = dac.cpp
OBJ = $(SRC:.cpp=.o)
EXE = dac

build: dac.cpp $(OBJ)
		$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJ)

run: $(EXE)
		./$(EXE) a.txt b.txt c.txt

%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f $(EXE) $(OBJ)