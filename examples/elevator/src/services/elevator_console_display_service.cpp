#include "elevator_console_display_service.h"
#include <iostream>

void ElevatorConsoleDisplayService::show_state(const ElevatorContext &ctx) {
    std::cout << "\n------------------------ \n"
              << "Elevator State:\n"
              << "------------------------ \n";

    std::cout << "State: " << ctx.name << "\n"
              << "Duration: " << ctx.duration << "s\n"
              << "Current Floor: " << ctx.current_floor << "\n"
              << "Target Floor: " << ctx.target_floor << "\n"
              << "Doors: [Open:" << ctx.doors.open
              << ", Opening:" << ctx.doors.opening
              << ", Closing:" << ctx.doors.closing << "]\n"
              << "Movement: [Up:" << ctx.movement.moving_up
              << ", Down:" << ctx.movement.moving_down
              << ", Stopped:" << ctx.movement.stopped << "]\n";

    if (ctx.emergency_active) {
        std::cout << "⚠️  EMERGENCY ACTIVE ⚠️\n";
    }

    if (ctx.obstacle_detected) {
        std::cout << "🚫 OBSTACLE DETECTED 🚫\n";
    }

    std::cout << std::endl;
}
