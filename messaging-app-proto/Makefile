CXX= clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

TARGET = main
SRC = main.cpp

all: $(TARGET)

$(TARGET) : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
