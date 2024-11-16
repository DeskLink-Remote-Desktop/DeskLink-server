CXX := g++
CXXFLAGS := -Wall -g
CXX_SUFFIX_FLAGS := -lstdc++

OBJ_DIR := obj/
BIN_DIR := bin/
SRC_DIR := src/
SUFFIX :=

BIN_NAME := DeskLink_server

ifeq ($(OS),Windows_NT)
	BIN_NAME := $(BIN_NAME).exe
	CXX_SUFFIX_FLAGS +=  -lws2_32
	SUFFIX := _win
endif

SRC_FILES := $(wildcard $(SRC_DIR)*.cpp)
HDR_FILES := $(wildcard $(SRC_DIR)*.hpp)
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%$(SUFFIX).o,$(SRC_FILES))

.PHONY: all clean run

all: $(BIN_DIR)$(BIN_NAME)

$(BIN_DIR)$(BIN_NAME): $(OBJ_FILES)
	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXX_SUFFIX_FLAGS)

$(OBJ_DIR)%$(SUFFIX).o: $(SRC_DIR)%.cpp $(SRC_FILES) $(HDR_FILES)
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXX_SUFFIX_FLAGS)

clean:
	rm -rf $(OBJ_DIR)* $(BIN_DIR)*

run: all
	@echo -n "Args: "
	@read args; \
	$(BIN_DIR)$(BIN_NAME) $$args
