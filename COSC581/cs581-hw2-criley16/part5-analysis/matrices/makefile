CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Wno-sign-compare

SRC = matrix_maker.cpp
OBJ = $(SRC:.cpp=.o)
EXE = mm

build: matrix_maker.cpp $(OBJ)
		$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJ)

run: $(EXE)
		./$(EXE) a.txt b.txt c.txt

%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f $(EXE) $(OBJ)