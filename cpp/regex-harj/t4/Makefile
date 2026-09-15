# ==============================
# Simple C++20 Makefile
# ==============================

MAIN := example.cpp

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic

TARGET := $(MAIN:.cpp=)

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CXX) $(CXXFLAGS) $(MAIN) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

