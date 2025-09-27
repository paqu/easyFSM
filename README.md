# State Machine Framework
[![C++](https://img.shields.io/badge/C%2B%2B-14-blue.svg)]()
[![CMake](https://img.shields.io/badge/CMake-3.16%2B-blue.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)]()

⭐ **If you find this project useful, please give it a star!** ⭐

## What is this?

A modern C++ framework for creating **state machine-based simulations**. This library simplifies building complex systems where behavior depends on current state and events - like traffic lights, elevators, game AI, or any system that transitions between different modes.

**Goal**: Make it easy to create, configure, and maintain state machine simulations without writing repetitive boilerplate code.

## Why use it?

- **Template-based**: Type-safe state and event definitions
- **Modular design**: Separate state logic, timing, and display concerns
- **Multiple examples**: Learn from traffic lights, elevators, and threaded implementations
- **Educational**: Clear code structure for understanding state machine patterns

## Project Structure

```
├── lib/                          # Core state machine framework
│   ├── include/state_machine/    # Header-only library
│   │   ├── core/                 # Base interfaces (IStateMachine, IActionHandler)
│   │   ├── implementations/      # Concrete classes (RuntimeStateMachine)
│   │   └── services/             # Support services (Timer, Display)
│   └── src/                      # Implementation files
├── examples/                     # Example applications
│   ├── traffic_light/            # Traffic light simulation
│   ├── elevator/                 # Elevator control system
│   └── traffic_light_threaded/   # Multithreaded traffic light
├── CMakeLists.txt               # Build configuration
├── Makefile                     # Convenience wrapper
└── build.sh                    # Build script
```

## How to Run

### Prerequisites
- C++14 compatible compiler (GCC 5.4+ or Clang 3.8+)
- CMake 3.16+
- pthread library

### Quick Start
```bash
# Build everything
make all

# Run examples
make run                          # Traffic light simulation
./build/examples/elevator/elevator_example
./build/examples/traffic_light_threaded/traffic_light_threaded
```

### Build Options

| Command | Description |
|---------|-------------|
| `make all` | Standard build (Release mode) |
| `make debug` | Debug build with symbols |
| `make clean` | Remove build files |
| `make rebuild` | Clean + build from scratch |
| `make run` | Build + run main traffic light example |
| `make test` | Build + run tests |
| `make asan` | Build with AddressSanitizer for memory debugging |
| `make help` | Show all available targets |

### Advanced Build Options
```bash
./build.sh --help               # Show all options
./build.sh --debug --clean      # Debug build from scratch
./build.sh --asan --tests       # AddressSanitizer + tests
./build.sh --run               # Build and run
```

## Examples

### 1. Traffic Light (`examples/traffic_light/`)

**Features:**
- Standard (European) vs Simple (American) traffic patterns
- Pedestrian crossing with button requests
- ASCII art visualization
- Configurable state timing

**Run:**
```bash
./build/examples/traffic_light/traffic_light_example
```

**Key states:** `CAR_GREEN`, `CAR_YELLOW`, `CAR_RED`, `WALK`, `WALK_PREP`, `WALK_FINISH`, `CAR_RED_YELLOW`

### 2. Elevator (`examples/elevator/`)

**Features:**
- Multi-floor navigation (configurable min/max floors)
- Door control with opening/closing/obstacle detection
- Emergency stop functionality
- Floor request queue management

**Run:**
```bash
./build/examples/elevator/elevator_example
```

**Key states:** `IDLE`, `DOORS_OPENING`, `DOORS_OPEN`, `DOORS_CLOSING`, `MOVING_UP`, `MOVING_DOWN`, `EMERGENCY_STOP`

### 3. Threaded Traffic Light (`examples/traffic_light_threaded/`)

**Features:**
- Real-time simulation with user input
- Modern C++17 threading (replacing legacy pthread)
- Interactive pedestrian button simulation
- Clean shutdown handling

**Run:**
```bash
./build/examples/traffic_light_threaded/traffic_light_threaded
```

**Controls:** Any key = pedestrian button, 'q' = quit

## How to Add Your Own Example

### 1. Create Directory Structure
```bash
mkdir -p examples/my_system/{src,include}
```

### 2. Define Your Domain Types
```cpp
// include/my_states.h
enum class MyState { IDLE, WORKING, ERROR };
enum class MyEvent { START, COMPLETE, FAIL };
```

### 3. Create Action Handler
```cpp
// include/my_action_handler.h
#include <state_machine/state_machine.h>

class MyActionHandler : public IActionHandler<MyState, MyEvent> {
public:
    void handle(MyState current, MyEvent event, MyState next) override {
        std::cout << "Transition: " << current << " -> " << next << std::endl;
        // Add your custom logic here
    }
};
```

### 4. Create Controller
```cpp
// include/my_controller.h
class MyController : public BaseController<MyState, MyEvent> {
public:
    MyController(/* dependencies */);
    void start_work() { handle_event(MyEvent::START); }
    void complete_work() { handle_event(MyEvent::COMPLETE); }
    void report_error() { handle_event(MyEvent::FAIL); }
};
```

### 5. Setup CMakeLists.txt
```cmake
# examples/my_system/CMakeLists.txt
add_executable(my_system_example
    src/my_controller.cpp
    src/my_action_handler.cpp
    main.cpp
)

target_include_directories(my_system_example PRIVATE include)
target_link_libraries(my_system_example PRIVATE state_machine_lib)
```

### 6. Add to Root CMakeLists.txt
```cmake
# Add this line to main CMakeLists.txt
add_subdirectory(examples/my_system)
```

### 7. Create Factory (Optional but Recommended)
```cpp
// include/my_factory.h
class MyFactory {
public:
    static std::unique_ptr<MyController> create_controller() {
        auto sm = std::make_shared<RuntimeStateMachine<MyState, MyEvent>>(MyState::IDLE);
        
        // Add transitions
        sm->add_transition(std::make_unique<SimpleStateTransition<MyState, MyEvent>>(
            MyState::IDLE, MyEvent::START, MyState::WORKING
        ));
        
        auto handler = std::make_shared<MyActionHandler>();
        return std::make_unique<MyController>(sm, handler);
    }
};
```

Look at existing examples for complete implementation patterns!

## Future Goals

### Short Term
- **Unit Testing Framework**: Comprehensive test suite with Google Test integration
- **Observer Pattern**: Decouple state change notifications from action handlers to reduce tight coupling between components

### Medium Term  
- **Runtime State/Event Creation**: Dynamic addition of new states and events without recompilation
- **DSL for Basic Structures**: Domain-specific language for generating common state machine patterns

### Long Term
- **Graphical State Machine Designer**: Visual tool for creating and editing state graphs and event flows
- **State Machine Validation**: Automatic detection of unreachable states and dead transitions
- **Performance Profiling**: Built-in metrics and timing analysis

## Contributing

We welcome contributions! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Follow existing code style and patterns
4. Add tests for new functionality
5. Update documentation
6. Submit a pull request

### Code Style Guidelines
- Use `snake_case` for variables and functions
- Use `PascalCase` for types and classes
- Follow RAII principles with smart pointers
- Document public interfaces with Doxygen-style comments

## License

**MIT License**

Copyright (c) 2025 Paweł Kusz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

⭐ **Found this helpful? Please star the repository to support the project!** ⭐

