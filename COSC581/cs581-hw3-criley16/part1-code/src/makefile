CXX=g++
CXXFLAGS = -Wall -g
TARGET = med_of_med
TARGET_DEL = med_of_med
SRCS = median_of_median.cpp
OBJS = $(SRCS:.cpp=.o)



all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(TARGET_DEL) $(OBJS)