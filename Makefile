CXX := -g++
# Compiler Flags
CXXFLAGS := -std=c++11 -pedantic-errors -Wall -Wextra -Werror
# Linker Flags
LDFLAGS := -L/usr/lib  -lstdc++ -lSDL2 -lGLEW -lGL -lfreetype
# Directories
BUILD := ./build
OBJ_DIR := $(BUILD)/objects
APP_DIR := $(BUILD)/executables
RESOURCES_DIR := $(APP_DIR)/resources
TARGET := main
INCLUDE := -Iinclude/ 
SRC := $(wildcard src/renderer/*.cpp) 	\
    $(wildcard src/physics/*.cpp)	\
    $(wildcard src/*.cpp)	      	\

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(RESOURCES_DIR)
	@cp -r src/resources/. $(RESOURCES_DIR)

debug: CXXFLAGS += -DDBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
