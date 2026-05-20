CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Wno-sign-compare

SRC = strassen.cpp
OBJ = $(SRC:.cpp=.o)
EXE = strassen

build: strassen.cpp $(OBJ)
		$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJ)

run: $(EXE)
		./$(EXE) a.txt b.txt c.txt

%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f $(EXE) $(OBJ)