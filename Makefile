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
	src/application_manager.cpp \
	src/browser_manager.cpp \
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


QT_CXXFLAGS = $(shell pkg-config --cflags Qt6Widgets)
QT_LDFLAGS = $(shell pkg-config --libs Qt6Widgets)

GUI_TARGET = build/voicedesk-gui

GUI_SOURCES = \
	src/gui_main.cpp \
	src/gui/main_window.cpp \
	src/command_parser.cpp \
	src/command_executor.cpp \
	src/application_manager.cpp \
	src/browser_manager.cpp \
	src/process_manager.cpp \
	src/file_manager.cpp \
	src/system_monitor.cpp \
	src/safety_engine.cpp \
	src/voice_engine.cpp \
	src/device_driver_client.cpp

GUI_OBJECTS = $(GUI_SOURCES:src/%.cpp=build/gui_%.o)

$(GUI_TARGET): $(GUI_OBJECTS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) \
		$(GUI_OBJECTS) \
		$(LDFLAGS) \
		$(LDLIBS) \
		$(QT_LDFLAGS) \
		-o $(GUI_TARGET)

build/gui_%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) \
		-c $< -o $@

gui: $(GUI_TARGET)

.PHONY: gui
