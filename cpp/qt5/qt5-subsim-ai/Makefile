# Makefile for the single-file Qt5 triangle app.
# Builds directly with moc + g++ + pkg-config (no qmake required).

TARGET   := triangle_app
SOURCE   := main.cpp
MOC_FILE := main.moc

CXX      := g++
MOC      := moc
CXXFLAGS := -std=c++14 -Wall -fPIC $(shell pkg-config --cflags Qt5Widgets)
LIBS     := $(shell pkg-config --libs Qt5Widgets)

.PHONY: all clean run

all: $(TARGET)

# main.cpp does `#include "main.moc"`, so it must exist before compiling.
$(MOC_FILE): $(SOURCE)
	$(MOC) $(SOURCE) -o $(MOC_FILE)

$(TARGET): $(SOURCE) $(MOC_FILE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(MOC_FILE)
