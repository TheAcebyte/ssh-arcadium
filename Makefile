CXX = g++

VCPKG_ROOT = $(HOME)/vcpkg
VCPKG_INSTALLED = $(CURDIR)/vcpkg_installed/x64-linux

CXXFLAGS = -std=c++23 -Wall -Wextra -O2 -MMD -MP -I$(VCPKG_INSTALLED)/include -Isrc
LDFLAGS = -L$(VCPKG_INSTALLED)/lib -lftxui-component -lftxui-dom -lftxui-screen

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

LIB_SRCS = $(wildcard $(SRC_DIR)/lib/*/*.cpp)
LIB_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))

LAUNCHER_SRCS = $(wildcard $(SRC_DIR)/launcher/*.cpp)
LAUNCHER_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LAUNCHER_SRCS))
LAUNCHER_BIN = $(BIN_DIR)/launcher

GAME_SHARED_SRCS = $(wildcard $(SRC_DIR)/game/shared/*.cpp)
GAME_SHARED_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(GAME_SHARED_SRCS))

GAME_CLIENT_SRCS = $(wildcard $(SRC_DIR)/game/client/*.cpp)
GAME_CLIENT_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(GAME_CLIENT_SRCS))
GAME_CLIENT_BIN = $(BIN_DIR)/game/client

GAME_SERVER_SRCS = $(wildcard $(SRC_DIR)/game/server/*.cpp)
GAME_SERVER_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(GAME_SERVER_SRCS))
GAME_SERVER_BIN = $(BIN_DIR)/game/server

BINS = $(LAUNCHER_BIN) $(GAME_CLIENT_BIN) $(GAME_SERVER_BIN)
OBJS = $(LIB_OBJS) $(LAUNCHER_OBJS) $(GAME_SHARED_OBJS) $(GAME_CLIENT_OBJS) $(GAME_SERVER_OBJS)

.PHONY: all dirs vcpkg-install clean clean-all

all: vcpkg-install dirs $(BINS)

vcpkg-install:
	if [ ! -d "$(VCPKG_ROOT)" ]; then \
		git clone https://github.com/microsoft/vcpkg.git $(VCPKG_ROOT); \
		$(VCPKG_ROOT)/bootstrap-vcpkg.sh; \
	fi
	cd $(VCPKG_ROOT) && ./vcpkg install --x-manifest-root=$(CURDIR)

dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/lib
	mkdir -p $(BUILD_DIR)/lib/block-canvas
	mkdir -p $(BUILD_DIR)/lib/text-cycler
	mkdir -p $(BUILD_DIR)/launcher
	mkdir -p $(BUILD_DIR)/game/shared
	mkdir -p $(BUILD_DIR)/game/client
	mkdir -p $(BUILD_DIR)/game/server
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/game

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LAUNCHER_BIN): $(LAUNCHER_OBJS) $(LIB_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(GAME_CLIENT_BIN): $(GAME_CLIENT_OBJS) $(GAME_SHARED_OBJS) $(LIB_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(GAME_SERVER_BIN): $(GAME_SERVER_OBJS) $(GAME_SHARED_OBJS) $(LIB_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	
clean-all: clean
	rm -rf $(VCPKG_ROOT)

compile_commands.json:
	bear -- make clean all

-include $(OBJS:.o=.d)
