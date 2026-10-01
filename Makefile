CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra \
	-Iinclude \
	-Ithird_party/whisper.cpp/include \
	-Ithird_party/whisper.cpp/ggml/include

WHISPER_DIR = third_party/whisper.cpp/build/bin

LDFLAGS = \
	-L$(WHISPER_DIR) \
	-Wl,-rpath,$(WHISPER_DIR)

LDLIBS = \
	-lwhisper \
	-lggml \
	-lggml-base \
	-lggml-cpu

TARGET = build/voicedesk

SOURCES = \
	src/main.cpp \
	src/command_parser.cpp \
	src/command_executor.cpp \
	src/process_manager.cpp \
	src/file_manager.cpp \
	src/system_monitor.cpp \
	src/safety_engine.cpp \
	src/voice_engine.cpp \
	src/device_driver_client.cpp

OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(OBJECTS) \
		$(LDFLAGS) $(LDLIBS) \
		-o $(TARGET)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
