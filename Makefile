CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2 -Isrc -MMD -MP

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

MAIN_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
MAIN_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MAIN_SRCS))
MAIN_BIN = $(BIN_DIR)/main

BINS = $(MAIN_BIN)
OBJS = $(MAIN_OBJS)

.PHONY: all dirs install clean

all: dirs $(BINS)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(MAIN_BIN): $(MAIN_OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(OBJS:.o=.d)
