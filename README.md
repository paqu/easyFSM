# Traffic Light Simulator
[![C++](https://img.shields.io/badge/C%2B%2B-14-blue.svg)]()

## Description
A C++ simulation of a traffic light system with pedestrian crossings. The project models traffic light states, state transitions, and pedestrian button requests, providing a flexible framework to handle timers, display services, and customizable state behaviors. It demonstrates modern C++ design patterns, including dependency injection, state machines, and action handlers.

## Features
- Simulates traffic lights with car and pedestrian signals.  
- Handles pedestrian button requests and pending states.  
- Uses a configurable state machine for flexible traffic light behavior.  
- Supports custom display and timer services.  
- Modular architecture with separate controllers and action handlers.

## Use Case
This simulator can be used for educational purposes, testing traffic light algorithms, or as a base for more complex traffic management simulations.

## 🏗️ Architecture

### Design Patterns Used
- **State Machine Pattern**: Runtime configurable state transitions
- **Factory Pattern**: Traffic light controller creation
- **Strategy Pattern**: Pluggable display and timer services
- **Observer Pattern**: Event-driven state changes
- **Dependency Injection**: Interface-based component integration

### Project Structure
```
├── include/
│   ├── interfaces/          # Abstract interfaces
│   ├── models/              # Data structures and enums
│   ├── controllers/         # Business logic controllers
│   ├── implementations/     # Concrete implementations
│   ├── factories/           # Object creation patterns
│   └── utils/              # Utility classes
├── src/
│   ├── controllers/        # Controller implementations
│   ├── implementations/    # Service implementations
│   ├── models/            # Model implementations
│   ├── factories/         # Factory implementations
│   └── utils/             # Utility implementations
└── main.cpp               # Application entry point
```

## 🚀 Getting Started

### Prerequisites
- **C++14** compatible compiler (GCC 5.4+ or Clang 3.8+)
- **Make** build system
- **pthread** library (typically included with GCC/Clang)
- ANSI terminal for colored ASCII display (optional)

### Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd traffic-light-simulator
   ```

2. **Build the project**
   ```bash
   make all
   ```

3. **Run the simulator**
   ```bash
   make run
   ```

### Build Options

```bash
# Standard build
make all

# Debug build with symbols
make debug

# Clean build files
make clean

# Complete clean (removes directories)
make distclean

# Rebuild from scratch
make rebuild

# Show help
make help
```

## 🎮 Usage

### Running the Simulator

When you start the application, you'll be prompted to choose a mode:

1. **Demo Mode**: Demonstrates both Standard and Simple traffic light types with automated transitions
2. **Simulation Mode**: Interactive real-time simulation with user controls

### Interactive Controls (Simulation Mode)
- **Any key**: Press pedestrian crossing button
- **'q'**: Quit the simulation

### Traffic Light Types

#### Standard Traffic Light
Includes all states: `GREEN → YELLOW → RED → RED_YELLOW → GREEN`
- Features the European-style RED+YELLOW preparation phase
- Full pedestrian crossing cycle when requested

#### Simple Traffic Light
Simplified cycle: `GREEN → YELLOW → RED → GREEN`
- No RED_YELLOW state (American-style)
- Extended yellow duration to compensate
- Streamlined pedestrian integration

### Creating Custom Controllers

You can create custom traffic light controllers using the factory or by manually configuring components:

#### Using the Factory (Recommended)
```cpp
#include "traffic_light_factory.h"
#include "ascii_display_service.h"
#include "function_timer_service.h"

// Create a timer service with custom callback
auto timer_service = std::make_unique<FunctionTimerService>([](uint32_t duration) {
    std::cout << "Starting " << duration << "s timer\n";
    // Your timer implementation here
});

// Create display service
auto display_service = std::make_unique<AsciiDisplayService>();

// Create controller using factory
auto controller = TrafficLightFactory::create_controller(
    TrafficLightType::STANDARD,  // or TrafficLightType::SIMPLE
    std::move(display_service),
    std::move(timer_service)
);

// Use the controller
controller->timeout_expired();  // Trigger state transition
controller->button_pressed();   // Pedestrian button press
```

#### Manual Configuration
```cpp
#include "runtime_state_machine.h"
#include "traffic_light_action_handler.h"
#include "traffic_light_controller.h"
#include "simple_state_transition.h"

// Create state machine
auto state_machine = std::make_shared<RuntimeStateMachine>(TrafficState::CAR_GREEN);

// Create action handler with services
auto action_handler = std::make_shared<TrafficLightActionHandler>(
    std::make_unique<ConsoleDisplayService>(),
    std::make_unique<FunctionTimerService>([](uint32_t duration) {
        // Custom timer logic
    })
);

// Configure transitions
state_machine->add_transition(std::make_unique<SimpleStateTransition>(
    TrafficState::CAR_GREEN, 
    SystemEvent::TIME_EXPIRED, 
    TrafficState::CAR_YELLOW
));

// Create controller
auto controller = std::make_unique<TrafficLightController>(
    state_machine, 
    action_handler
);
```

### Custom Display Services
Implement the `IDisplayService` interface to create custom visualizations:
```cpp
class CustomDisplayService : public IDisplayService {
public:
    void show_state(const StateContext &ctx) override {
        // Custom visualization logic
        std::cout << "Custom: " << ctx.name 
                  << " for " << ctx.duration << "s\n";
    }
    
    void show_button_state(bool is_new_request) override {
        std::cout << (is_new_request ? "Button pressed!" : "Waiting...") << "\n";
    }
};
```

### Custom Timer Services
Create custom timer implementations by implementing `ITimerService`:
```cpp
class CustomTimerService : public ITimerService {
private:
    std::function<void()> callback;
    
public:
    CustomTimerService(std::function<void()> cb) : callback(cb) {}
    
    void start_timeout(uint32_t duration_sec) override {
        // Your timer implementation (could use std::thread, signals, etc.)
        std::thread([this, duration_sec]() {
            std::this_thread::sleep_for(std::chrono::seconds(duration_sec));
            if (callback) callback();
        }).detach();
    }
};
```

## 🐛 Known Issues

### Current Limitations
1. **Terminal Dependencies**: ASCII display requires ANSI-compatible terminal
2. **Platform-Specific**: pthread implementation limits portability to POSIX systems
3. **Input Buffering**: Console input may be buffered on some systems
4. **Memory Management**: Some raw pointer usage in callback functions

## 🤝 Contributing

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Follow the existing code style
4. Add appropriate documentation
5. Submit a pull request

### Code Style Guidelines
- Use meaningful variable and function names
- Follow RAII principles
- Prefer const correctness
- Document public interfaces
- Use smart pointers for dynamic allocation

