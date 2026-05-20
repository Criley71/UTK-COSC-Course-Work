CXX=g++
CXXFLAGS = -Wall -g
TARGET = array_gen
TARGET_DEL = array_gen
SRCS = array_generator.cpp
OBJS = $(SRCS:.cpp=.o)



all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(TARGET_DEL) $(OBJS)