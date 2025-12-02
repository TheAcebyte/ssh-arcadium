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

BINS = $(LAUNCHER_BIN)
OBJS = $(LIB_OBJS) $(LAUNCHER_OBJS)

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
	mkdir -p $(BIN_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LAUNCHER_BIN): $(LAUNCHER_OBJS) $(LIB_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	
clean-all: clean
	rm -rf $(VCPKG_ROOT)

compile_commands.json:
	bear -- make clean all

-include $(OBJS:.o=.d)
