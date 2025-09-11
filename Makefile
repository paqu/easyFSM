# Makefile for Traffic Light Simulator
# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -pthread -g -O2 -MMD -MP
LDFLAGS = -pthread

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Program name
TARGET = $(BIN_DIR)/traffic_light_simulator

# Source subdirectories
SRC_SUBDIRS = models \
              implementations/services \
              implementations/state_machines \
              implementations/transitions \
              implementations/handlers \
              controllers \
              utils \
			  factories

# Find all source files in subdirectories + main.cpp
SOURCES = $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(SRC_DIR)/$(dir)/*.cpp)) \
          $(SRC_DIR)/main.cpp

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Dependency files
DEPS = $(OBJECTS:.o=.d)

# Object subdirectories
OBJ_SUBDIRS = $(addprefix $(OBJ_DIR)/,$(SRC_SUBDIRS))

# Include paths - all subdirectories for clean includes
INCLUDES = -I$(INC_DIR) \
           -I$(INC_DIR)/interfaces \
           -I$(INC_DIR)/models \
           -I$(INC_DIR)/implementations/services \
           -I$(INC_DIR)/implementations/state_machines \
           -I$(INC_DIR)/implementations/transitions \
           -I$(INC_DIR)/implementations/handlers \
           -I$(INC_DIR)/controllers \
           -I$(INC_DIR)/utils \
           -I$(INC_DIR)/factories

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# Main target
all: directories $(TARGET)
	@echo "$(GREEN)Build completed successfully!$(NC)"

# Create directories if they don't exist
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_SUBDIRS)

# Linking
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Linking $@...$(NC)"
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "$(GREEN)Executable created: $@$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Include dependency files if they exist (MUST be after compilation rule)
-include $(DEPS)

# Clean build files
clean:
	@echo "$(RED)Cleaning build files...$(NC)"
	rm -rf $(OBJECTS) $(DEPS) $(TARGET)
	@echo "$(GREEN)Clean completed!$(NC)"

# Full clean (removes directories too)
distclean: clean
	@echo "$(RED)Removing directories...$(NC)"
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: all
	@echo "$(GREEN)Running Traffic Light Simulator...$(NC)"
	@./$(TARGET)



# Help
help:
	@echo "$(GREEN)Available targets:$(NC)"
	@echo "  make all       - Build the project (default)"
	@echo "  make clean     - Remove object files and executable"
	@echo "  make distclean - Remove all generated files and directories"
	@echo "  make run       - Build and run the simulator"
	@echo "  make rebuild   - Clean and rebuild"
	@echo "  make debug     - Build with debug symbols"
	@echo "  make help      - Show this help message"

# Debug build
debug: CXXFLAGS += -DDEBUG -g3 -O0
debug: clean all

# Rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean distclean run help debug rebuild directories

