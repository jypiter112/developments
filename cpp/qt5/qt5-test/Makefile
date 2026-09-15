# Makefile for the Qt "Triangle Clock" example.
#
# Builds directly with g++ / pkg-config / moc -- no qmake required.
#
# Defaults to Qt5. If you're on a Qt6-only system, build with:
#   make QT_PKG=Qt6Widgets MOC=moc6
# (adjust MOC to whatever your distro calls the Qt6 moc binary, e.g.
# /usr/lib/qt6/libexec/moc or qt6-moc)

QT_PKG   ?= Qt5Widgets
MOC      ?= moc

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -fPIC $(shell pkg-config --cflags $(QT_PKG))
LDFLAGS  := $(shell pkg-config --libs $(QT_PKG))

TARGET   := triangle_clock

# Plain C++ sources.
SOURCES     := main.cpp Triangle.cpp RenderWidget.cpp
# Headers that declare Q_OBJECT and therefore need moc-generated code.
MOC_HEADERS := RenderWidget.h

OBJECTS     := $(SOURCES:.cpp=.o)
MOC_SOURCES := $(MOC_HEADERS:.h=_moc.cpp)
MOC_OBJECTS := $(MOC_SOURCES:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) $(MOC_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%_moc.cpp: %.h
	$(MOC) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS) $(MOC_OBJECTS) $(MOC_SOURCES)
