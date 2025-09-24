
#include "console_display_service.h"

#include "traffic_context.h"
#include <iostream>

void ConsoleDisplayService::show_state(const TrafficContext &ctx) {
    std::cout << "\n------------------------ \n"
              << "Traffic Light State:\n"
              << "------------------------ \n";

    std::cout << "State: " << ctx.name << "\n"
              << "Duration: " << ctx.duration << "s\n"
              << "Car Lights: [R:" << ctx.carLights.red
              << ", Y:" << ctx.carLights.yellow << ", G:" << ctx.carLights.green
              << "]\n"
              << "Pedestrian Lights: [R:" << ctx.pedLights.red
              << ", G:" << ctx.pedLights.green << "]\n";

    std::cout << std::endl;
}
