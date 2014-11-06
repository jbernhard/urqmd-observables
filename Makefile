CXX = $(shell which icpc &> /dev/null && echo icpc || echo g++)
CXXFLAGS = -std=c++11 -march=native -O3 -Wall
TARGET = urqmd-observables
SRC = $(TARGET).cxx

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET)
