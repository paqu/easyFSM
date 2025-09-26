#include <iostream>
#include <state_machine/state_machine.h>

// Elevator domain types
#include "controllers/elevator_controller.h"
#include "factories/elevator_factory.h"
#include "services/elevator_console_display_service.h"

using namespace state_machine;

int main() {
    std::cout << "=== Elevator Example (using state_machine library) ==="
              << std::endl;

    // Create timer function
    std::function<void(uint32_t)> timer_func = [](uint32_t duration) {
        std::cout << "Timer started for " << duration << " seconds"
                  << std::endl;
    };

    // Create display service (simple console output for now)
    auto display_service = std::make_unique<ElevatorConsoleDisplayService>();

    // Create controller using factory
    auto controller = ElevatorFactory::create_controller(
        ElevatorType::BASIC, std::move(display_service),
        std::make_unique<FunctionTimerService>(timer_func),
        0, // min floor
        5  // max floor
    );

    std::cout << "\n=== Testing Elevator Operation ===" << std::endl;

    // Test basic floor request
    std::cout << "\n1. Request floor 3:" << std::endl;
    controller->request_floor(3);
    controller->timer_expired(); // IDLE -> DOORS_OPENING
    controller->timer_expired(); // DOORS_OPENING -> DOORS_OPEN
    controller->timer_expired(); // DOORS_OPEN -> DOORS_CLOSING
    controller->timer_expired(); // DOORS_CLOSING -> MOVING_UP
    controller->floor_reached(); // Simulate reaching floor 3
    controller->timer_expired(); // MOVING_UP -> DOORS_OPENING

    std::cout << "\n2. Test doors control:" << std::endl;
    controller->open_doors();  // Force doors open
    controller->close_doors(); // Force doors close

    std::cout << "\n3. Test emergency:" << std::endl;
    controller->request_floor(1);
    controller->emergency_stop(); // Emergency stop
    controller->timer_expired();  // EMERGENCY_STOP -> IDLE (after timeout)

    std::cout << "\n=== Elevator Example completed successfully! ==="
              << std::endl;
    return 0;
}
