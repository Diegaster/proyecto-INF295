CXX = g++
CXXFLAGS = -std=c++17 -O2

SRC = src/main.cpp \
      src/LectorInstancias.cpp \
      src/Evaluador.cpp

TARGET = mdvrptw

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
