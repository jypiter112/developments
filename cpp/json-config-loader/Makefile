# ==============================
# Simple C++20 Makefile
# ==============================

MAIN := main.cpp

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic

TARGET := $(MAIN:.cpp=)

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CXX) $(CXXFLAGS) $(MAIN) -o mcat

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
