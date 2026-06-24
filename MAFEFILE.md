CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = mdvrptw

all: $(TARGET)

$(TARGET): $(OBJ)
$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
rm -f $(OBJ) $(TARGET)

run: $(TARGET)
./$(TARGET)
