CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = build/voicedesk

SOURCES = \
	src/main.cpp \
	src/command_parser.cpp \
	src/command_executor.cpp \
	src/process_manager.cpp \
	src/file_manager.cpp \
	src/system_monitor.cpp \
	src/safety_engine.cpp

OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
