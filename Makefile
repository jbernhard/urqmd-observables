CXX = $(shell which icpc &> /dev/null && echo icpc || echo g++)
CXXFLAGS = -std=c++11 -march=native -O3 -Wall
NAME = urqmd-observables
EXP = atlas
TARGET = $(NAME)-$(EXP)
SRC = $(NAME).cxx

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET)
