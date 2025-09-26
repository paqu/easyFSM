#include <iostream>
#include <state_machine/state_machine.h>

// Traffic light domain types
#include "controllers/traffic_light_controller.h"
#include "factories/traffic_light_factory.h"
#include "services/console_display_service.h"

using namespace state_machine;

int main() {
    std::cout << "=== Traffic Light Example (using state_machine library) ==="
              << std::endl;

    // Create timer function
    std::function<void(uint32_t)> timer_func = [](uint32_t duration) {
        std::cout << "Timer started for " << duration << " seconds"
                  << std::endl;
    };

    // Create controller using factory
    auto controller = TrafficLightFactory::create_controller(
        TrafficLightType::STANDARD, std::make_unique<ConsoleDisplayService>(),
        std::make_unique<FunctionTimerService>(timer_func));

    // Test basic operation
    std::cout << "\n=== Testing State Transitions ===" << std::endl;
    controller->timeout_expired(); // GREEN -> YELLOW
    controller->timeout_expired(); // YELLOW -> RED
    controller->timeout_expired(); // RED -> RED_YELLOW
    controller->timeout_expired(); // RED_YELLOW -> GREEN

    std::cout << "\n=== Testing Pedestrian Button ===" << std::endl;
    controller->button_pressed();  // Pedestrian request
    controller->timeout_expired(); // GREEN -> YELLOW
    controller->timeout_expired(); // YELLOW -> WALK_PREP (because pedestrian
                                   // requested)
    controller->timeout_expired(); // WALK_PREP -> WALK
    controller->timeout_expired(); // WALK -> WALK_FINISH
    controller->timeout_expired(); // WALK_FINISH -> RED_YELLOW

    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    return 0;
}
