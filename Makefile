# Makefile for Traffic Light Simulator
# Kompilator i flagi
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -pthread -g -O2
LDFLAGS = -pthread

# Katalogi
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Nazwa programu
TARGET = $(BIN_DIR)/traffic_light_simulator

# Pliki źródłowe i obiektowe
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Includes
INCLUDES = -I$(INC_DIR)

# Kolory dla output (opcjonalne)
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# Główny cel
all: directories $(TARGET)
	@echo "$(GREEN)Build completed successfully!$(NC)"

# Tworzenie katalogów jeśli nie istnieją
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

# Linkowanie
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Linking $@...$(NC)"
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "$(GREEN)Executable created: $@$(NC)"

# Kompilacja plików obiektowych
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(YELLOW)Compiling $<...$(NC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Czyszczenie
clean:
	@echo "$(RED)Cleaning build files...$(NC)"
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
	@echo "$(GREEN)Clean completed!$(NC)"

# Pełne czyszczenie (usuwa też katalogi)
distclean: clean
	@echo "$(RED)Removing directories...$(NC)"
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Uruchomienie programu
run: all
	@echo "$(GREEN)Running Traffic Light Simulator...$(NC)"
	@echo "$(YELLOW)Press any key for pedestrian button, 'q' to quit$(NC)"
	@./$(TARGET)

# Pomoc
help:
	@echo "$(GREEN)Available targets:$(NC)"
	@echo "  make all       - Build the project (default)"
	@echo "  make clean     - Remove object files and executable"
	@echo "  make distclean - Remove all generated files and directories"
	@echo "  make run       - Build and run the simulator"
	@echo "  make debug     - Build with debug symbols"
	@echo "  make help      - Show this help message"

# Debug build
debug: CXXFLAGS += -DDEBUG -g3 -O0
debug: clean all

# Rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean distclean run help debug rebuild directories

# Dependencies (automatyczne generowanie zależności)
DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MT '$(OBJ_DIR)/$*.o' $< > $@
