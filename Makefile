# Makefile - CMake Wrapper
# This Makefile provides the same interface as the original Makefile
# but uses CMake and build.sh under the hood for better build management

# Colors for output (same as original)
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# Build directory
BUILD_DIR = build
TARGET = $(BUILD_DIR)/bin/traffic_light_simulator

# Default target (equivalent to original 'all')
.PHONY: all clean distclean run help debug rebuild directories test

all: directories
	@echo "$(GREEN)Building Traffic Light Simulator with CMake...$(NC)"
	@./build.sh
	@echo "$(GREEN)Build completed successfully!$(NC)"

# Create directories (compatibility with original Makefile)
directories:
	@echo "$(YELLOW)Preparing build environment...$(NC)"

# Clean build files (equivalent to original clean)
clean:
	@echo "$(RED)Cleaning build files...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)Clean completed!$(NC)"

# Full clean (same as distclean in original)
distclean: clean
	@echo "$(RED)Removing all generated files...$(NC)"
	@rm -f compile_commands.json
	@echo "$(GREEN)Full clean completed!$(NC)"

# Run the program (equivalent to original run)
run: all
	@echo "$(GREEN)Running Traffic Light Simulator...$(NC)"
	@./build.sh --run

# Debug build (equivalent to original debug)
debug: 
	@echo "$(YELLOW)Building debug version...$(NC)"
	@./build.sh --debug --clean
	@echo "$(GREEN)Debug build completed!$(NC)"

# Rebuild (clean + build, equivalent to original rebuild)
rebuild: clean all

# Test target (new, for future unit tests)
test:
	@echo "$(YELLOW)Running tests...$(NC)"
	@./build.sh --tests --clean
	@echo "$(GREEN)Tests completed!$(NC)"

# Memory debugging with AddressSanitizer (new feature)
asan: 
	@echo "$(YELLOW)Building with AddressSanitizer...$(NC)"
	@./build.sh --asan --debug --clean
	@echo "$(GREEN)ASAN build completed!$(NC)"

# Quick run for development (build + run)
dev: 
	@echo "$(YELLOW)Development build and run...$(NC)"
	@./build.sh --clean --run

# Help target (improved from original)
help:
	@echo "$(GREEN)Available targets:$(NC)"
	@echo "  make all       - Build the project (default, uses CMake)"
	@echo "  make clean     - Remove build directory"
	@echo "  make distclean - Remove all generated files"
	@echo "  make run       - Build and run the simulator"
	@echo "  make rebuild   - Clean and rebuild"
	@echo "  make debug     - Build with debug symbols and run"
	@echo "  make test      - Build and run unit tests"
	@echo "  make asan      - Build with AddressSanitizer for memory debugging"
	@echo "  make dev       - Quick development cycle (clean + build + run)"
	@echo "  make help      - Show this help message"
	@echo ""
	@echo "$(YELLOW)Advanced options (call build.sh directly):$(NC)"
	@echo "  ./build.sh --help           - Show all build.sh options"
	@echo "  ./build.sh --debug --asan   - Debug build with memory sanitizer"
	@echo "  ./build.sh --build-dir tmp  - Use custom build directory"
	@echo ""
	@echo "$(GREEN)Migration info:$(NC)"
	@echo "  This Makefile is a wrapper around CMake and build.sh"
	@echo "  All original make targets work the same way"
	@echo "  CMake provides better dependency management and cross-platform support"

# Status target (new) - shows build information
status:
	@echo "$(GREEN)=== Build Status ===$(NC)"
	@echo "Build system: CMake (wrapper Makefile)"
	@echo "Build directory: $(BUILD_DIR)"
	@if [ -d "$(BUILD_DIR)" ]; then \
		echo "Status: Built ($(BUILD_DIR) exists)"; \
		if [ -f "$(TARGET)" ]; then \
			echo "Executable: Ready ($(TARGET))"; \
			ls -la $(TARGET); \
		else \
			echo "Executable: Missing"; \
		fi; \
	else \
		echo "Status: Not built (run 'make' to build)"; \
	fi
	@echo ""

# Info target - shows what changed from original Makefile
info:
	@echo "$(GREEN)=== Migration Information ===$(NC)"
	@echo "$(YELLOW)What's new:$(NC)"
	@echo "  • Using CMake for better build management"
	@echo "  • Automatic parallel compilation"
	@echo "  • Better dependency detection"
	@echo "  • Cross-platform support"
	@echo "  • AddressSanitizer support"
	@echo "  • Prepared for unit testing"
	@echo ""
	@echo "$(YELLOW)What's the same:$(NC)"
	@echo "  • All original make targets work identically"
	@echo "  • Same compiler flags and options"
	@echo "  • Same output structure"
	@echo "  • Same program behavior"
	@echo ""
	@echo "$(YELLOW)Quick comparison:$(NC)"
	@echo "  OLD: make all && make run"
	@echo "  NEW: make run  (same result, cleaner)"
	@echo ""

# Compatibility aliases (exactly like original Makefile)
build: all

# Advanced targets for power users
cmake-gui:
	@echo "$(YELLOW)Opening CMake GUI...$(NC)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake-gui ..

# Generate compile_commands.json for IDE support
compile-commands:
	@echo "$(YELLOW)Generating compile_commands.json for IDE support...$(NC)"
	@./build.sh
	@if [ -f "$(BUILD_DIR)/compile_commands.json" ]; then \
		cp $(BUILD_DIR)/compile_commands.json .; \
		echo "$(GREEN)compile_commands.json ready for your IDE!$(NC)"; \
	fi

# Performance profiling build
profile:
	@echo "$(YELLOW)Building with profiling support...$(NC)"
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS="-pg" ..
	@cd $(BUILD_DIR) && cmake --build . --parallel
	@echo "$(GREEN)Profile build ready! Run with: make run, then analyze with gprof$(NC)"

# Show build size
size:
	@if [ -f "$(TARGET)" ]; then \
		echo "$(GREEN)Executable size:$(NC)"; \
		ls -lah $(TARGET); \
		file $(TARGET); \
	else \
		echo "$(RED)No executable found. Run 'make' first.$(NC)"; \
	fi

# Default error message for unknown targets
%:
	@echo "$(RED)Unknown target: $@$(NC)"
	@echo "Run 'make help' to see available targets"
	@exit 1
